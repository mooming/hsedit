# VirtualTextBuffer Design Review (Revised)

## Overview

`VirtualTextBuffer` provides a virtualized view of a large text file by splitting it into fixed-size pages on disk, keeping only a subset loaded in memory (LRU cache). The user sees one continuous buffer of lines, while the implementation pages data in/out transparently.

**Key change from previous design:** Uses `PageCache` (from `PageCache.h`) as the in-memory page wrapper, with a Structure of Arrays (SoA) for page metadata.

## Architecture

```
VirtualTextBuffer
├── Cache Policy (anonymous struct)
│   ├── life                     — duration for incremental LRU eviction (seconds)
│   ├── incrementalLRUPeriod     — time interval for incremental LRU update (seconds)
│   ├── cacheSizeInBytes         — cache size hint in bytes
│   └── pageSizeInBytes          — initial page size in bytes (halves if exceeds 2x)
│
├── Page Table (SoA - anonymous struct)
│   ├── pageLineStart[]  — line index where each page begins
│   ├── pageLength[]     — number of lines per page
│   └── pageFiles[]      — disk path for each page file
│
├── Page Cache
│   └── std::vector<PageCache>  — in-memory pages
│       ├── TIndex index
│       ├── TTimestamp lastTimeTouched
│       └── TextBuffer buffer
│
└── Configuration
    ├── storagePath       — base file path for page generation
    └── Open()/Close()    — lifecycle management
```

### Key Design Decisions

1. **Structure of Arrays (SoA)** for page metadata — cache-friendly iteration over page properties
2. **PageCache wrapper** — each in-memory page carries its index, access timestamp (steady_clock), and TextBuffer
3. **LRU via Touch/Update_Async** — pages touched via `Touch(pageId)`; `Update_Async(deltaTime)` called by `VirtualTextBufferSystem` performs incremental LRU eviction based on `life` and `incrementalLRUPeriod`
4. **Byte-based page sizing** — `pageSizeInBytes` is initial size; pages halve if they exceed 2x this value
5. **Open/Close lifecycle** — `Open()` loads page files from disk into the cache; `Close()` collapses all pages back into the single base file
6. **Prefetch** — `Prefetch(pageId)` submits an asynchronous request to load a page for future use
7. **No copy, move-only** — ownership semantics for resource-heavy buffer

---

## Issues Found

### Critical

1. **No dirty tracking in PageCache** — `PageCache` has no `IsModified` flag. `UnloadPage` cannot know whether to write back to disk. Every unload would either always write (wasteful) or never write (data loss). **Recommendation:** Add `bool modified` to `PageCache` or track dirty pages separately.

2. **`GetLine` returns `const TLine&`** — if the requested line is not in memory, it loads the page first via `EnsurePageLoaded`. But the caller gets a reference to internal memory that could be invalidated by the next call (e.g., another `GetLine` that triggers eviction). This is a classic invalidation bug. **Recommendation:** Return by value, or document that the reference is only valid until the next modification.

3. **`Split` is marked `const`** — but it returns a new `VirtualTextBuffer` with a subset of the page table. If the original is modified after split, the split buffer may become inconsistent. **Recommendation:** Either remove `const` (if split modifies original) or document that split creates an independent copy (which it does, but the page table sharing needs clarification).

4. **`Open()` behavior ambiguous** — Does it:
   - Read existing page files and populate the cache?
   - Create new empty page files?
   - Overwrite existing files?
   - **Recommendation:** Clarify: `Open()` should load existing page files if they exist, otherwise create initial empty page. Document this explicitly.

5. **`Close()` semantics** — "Collapse all pages into the single base file" suggests merging. But what about pages that weren't modified? **Recommendation:** `Close()` should:
   - Flush all dirty pages to their page files
   - Optionally merge all page files back into the base file (current behavior)
   - Clear the in-memory cache

### Medium

6. **Anonymous struct fields are public by default** — `pageLineStart`, `pageLength`, `pageFiles`, `life`, `incrementalLRUPeriod`, `cacheSizeInBytes`, `pageSizeInBytes` are all publicly accessible. While this is intentional for SoA, it should be documented that these are internal state and should not be modified directly.

7. **`LoadPageFromDisk` is `const` but loads into non-const buffer** — The method signature is `void LoadPageFromDisk(TPageIndex pageIndex, TLineIndex startLine) const`, but it loads data into the non-const `buffer` vector. **Recommendation:** Remove `const` qualifier.

8. **`Prefetch` vs `EnsurePageLoaded`** — `Prefetch` is an asynchronous request to load a page for future use (not immediate). `EnsurePageLoaded` guarantees the page is loaded now (evicting if necessary). **Recommendation:** Implement `Prefetch` as a queue of pending load requests, processed during `Update_Async` or by `VirtualTextBufferSystem`.

9. **`Update_Async` called by `VirtualTextBufferSystem`** — This creates a coupling between `VirtualTextBuffer` and `VirtualTextBufferSystem`. The system calls `Update_Async` to perform LRU updates. **Recommendation:** Document this relationship clearly. Consider making `Update_Async` protected or private if the system is the only caller.

10. **Page halving logic** — "Page will be halved if its size exceeds double of this value" — this suggests dynamic page sizing. **Recommendation:** Implement and document the halving logic clearly. When does halving occur? Is it triggered by `AddLine` or by a periodic check?

### Minor

11. **`buffer` member name** — `std::vector<PageCache> buffer` is a bit generic. `cachedPages` or `pageCache` might be more descriptive.

12. **`TFilePath` alias** — HSTypes defines `TFilePath` as `std::filesystem::path`. Consistent use is good, but consider whether the alias adds clarity or just noise.

13. **No thread safety** — LRU cache, page table, and disk I/O all need synchronization if used from multiple threads. Not necessarily required now, but should be documented.

14. **`NumLines()` complexity** — Currently O(1) if we cache the total, or O(n) if we sum `pageLength`. **Recommendation:** Cache the total line count for O(1) access.

---

## Plan

### Phase 1: Foundation (Core Data Structures)

| Step | Task | Effort |
|------|------|--------|
| 1.1 | Add dirty tracking to `PageCache` or maintain separate dirty set | Small |
| 1.2 | Fix `LoadPageFromDisk` const-correctness (remove `const`) | Small |
| 1.3 | Add documentation for anonymous struct fields (SoA internal state) | Small |
| 1.4 | Cache total line count for O(1) `NumLines()` | Small |

### Phase 2: Core Routing (FindPage + GetLine)

| Step | Task | Effort |
|------|------|--------|
| 2.1 | Implement `FindPage(lineNumber)` — binary search over `pageLineStart` | Medium |
| 2.2 | Implement `GetLine(lineNumber)` — find page, ensure loaded, return line | Medium |
| 2.3 | Implement `NumLines()` — return cached total | Trivial |
| 2.4 | Implement `NumPages()` — `pageLength.size()` | Trivial |
| 2.5 | Implement `NumLoadedPages()` — `buffer.size()` | Trivial |

### Phase 3: LRU Cache (via Update_Async)

| Step | Task | Effort |
|------|------|--------|
| 3.1 | Implement `Touch(pageId)` — update `lastTimeTouched` on PageCache | Small |
| 3.2 | Implement `EvictLRU()` — find oldest PageCache by `lastTimeTouched`, unload | Small |
| 3.3 | Implement `EnsurePageLoaded(pageId)` — load if missing, evict if over capacity | Medium |
| 3.4 | Implement `UnloadPage(pageId)` — write back if dirty, remove from cache | Medium |
| 3.5 | Implement `Update_Async(deltaTime)` — incremental LRU sweep based on `life` and `incrementalLRUPeriod` | Medium |
| 3.6 | Implement `Prefetch(pageId)` — queue asynchronous load request for future use | Small |

### Phase 4: Mutation Operations

| Step | Task | Effort |
|------|------|--------|
| 4.1 | Implement `AddLine(line)` — append to last page or create new page (with halving check) | Medium |
| 4.2 | Implement `InsertLine(lineNo, line)` — find page, insert, adjust indices | Large |
| 4.3 | Implement `ReplaceLine(lineNo, line)` — find page, replace | Medium |
| 4.4 | Implement `RemoveLine(lineNo)` — find page, remove, adjust indices | Medium |
| 4.5 | Implement `ExtractLine(lineNo)` — RemoveLine + return value | Small |

### Phase 5: Persistence (Open/Close)

| Step | Task | Effort |
|------|------|--------|
| 5.1 | Implement `GetPageFilePath(pageId)` — `<dir>/<stem>_page_<id>.txt` | Small |
| 5.2 | Implement `SavePageToDisk(pageId)` — write TextBuffer to file | Medium |
| 5.3 | Implement `Open()` — load existing page files, populate page table and cache | Large |
| 5.4 | Implement `Close()` — flush dirty pages, collapse to base file, clear cache | Large |

### Phase 6: Advanced Operations

| Step | Task | Effort |
|------|------|--------|
| 6.1 | Implement `Split(splitLine)` — return new buffer with pages after splitLine | Large |
| 6.2 | Document `Update_Async` coupling with `VirtualTextBufferSystem` | Small |
| 6.3 | Add thread safety (mutex) if multi-threaded access is expected | Medium |

### Phase 7: Verification

| Step | Task | Effort |
|------|------|--------|
| 7.1 | Write unit tests for FindPage, GetLine, AddLine, InsertLine, RemoveLine | Large |
| 7.2 | Test LRU eviction via Update_Async with time simulation | Medium |
| 7.3 | Test persistence roundtrip (Open → Modify → Close → Open) | Large |
| 7.4 | Test page halving when size exceeds 2x pageSizeInBytes | Medium |
| 7.5 | Benchmark against sequential access patterns | Medium |

---

## VirtualTextBufferSystem Integration

`VirtualTextBuffer` is designed to be managed by `VirtualTextBufferSystem`, which:
- Calls `Update_Async(deltaTime)` periodically (e.g., from game loop or worker thread)
- Manages multiple `VirtualTextBuffer` instances
- Handles file watching and change notifications
- Coordinates LRU across multiple buffers

The coupling is intentional: `VirtualTextBuffer` handles per-buffer caching, while `VirtualTextBufferSystem` handles multi-buffer coordination.
