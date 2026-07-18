# VirtualTextBuffer Design Review

## Overview

`VirtualTextBuffer` provides a virtualized view of a large text file by splitting it into fixed-size pages on disk, keeping only a subset loaded in memory (LRU cache). The user sees one continuous buffer of lines, while the implementation pages data in/out transparently.

## Architecture

```
VirtualTextBuffer
├── Page Table (SoA)
│   ├── pageLineStart[]  — line index where each page begins
│   ├── pageLength[]     — number of lines per page
│   └── pageFiles[]      — disk path for each page file
│
├── Page Cache
│   └── std::vector<PageCache>  — in-memory pages (LRU order)
│
└── Configuration
    ├── maxPages          — LRU capacity (0 = unlimited)
    └── storagePath       — base file path for page generation
```

### Key Design Decisions

1. **Structure of Arrays (SoA)** for page metadata — cache-friendly iteration over page properties
2. **LRU page cache** — pages evicted by least-recently-touched, with disk persistence on unload
3. **PageCache wrapper** — each in-memory page carries its index, access timestamp, and TextBuffer
4. **No copy, move-only** — ownership semantics for resource-heavy buffer

---

## Issues Found

### Critical

1. **Page table uses anonymous struct with no accessors** — fields (`pageLineStart`, `pageLength`, `pageFiles`) are public by default. Any internal method can corrupt them. Should be private with getters, or the struct itself should be private.

2. **`std::vector<PageCache>` used as LRU cache** — acceptable for bounded caches (editor typically < 100 pages). PageCache is cheap to move (index + timestamp + moved TextBuffer = O(1)), so vector shifts are negligible. Still need a hash map for O(1) `IsPageLoaded` lookup.

3. **`FindPage(TLineIndex lineNumber)` has no implementation** — the binary search over `pageLineStart` is the heart of the routing logic. Without it, `GetLine`, `AddLine`, `InsertLine`, etc. cannot work.

4. **`EnsurePageLoaded` has no implementation** — the LRU load-with-eviction logic is the core async boundary.

### Medium

5. **`Open()` and `Close()` ambiguous** — `Open()` says "open a file and generate page files" but doesn't specify: does it read the existing file into pages? Create new empty pages? Overwrite? Should this be two methods: `LoadFromDisk()` and `InitializeEmpty()`.

6. **`Close()` says "collapse all pages into the single base file"** — this is a merge operation, not a close. Better named `MergeToBase()` or `SaveAll()`. Also unclear: does Close() flush dirty pages first?

7. **No dirty tracking** — `PageCache` has no `IsModified` flag. Without it, `UnloadPage` cannot know whether to write back to disk. Every unload would either always write (wasteful) or never write (data loss).

8. **`GetLine` returns `const TLine&`** — if the requested line is not in memory, it loads the page first. But the caller gets a reference to internal memory that could be invalidated by the next call. This is a classic invalidation bug. Either return by value, or document that the reference is only valid until the next modification.

9. **`ExtractLine` returns `TLine` by value** — this is correct (move semantics), but the side effect (removing the line) on a const-looking operation may surprise callers. The name "Extract" implies removal, which is fine, but the class should be consistent — `RemoveLine` already exists and does the same thing without returning the value.

10. **`Split` returns `unique_ptr<VirtualTextBuffer>`** — this is a structural operation (splits the original). But it's marked `const`. Does it modify the original? If the original is split, its page table changes. Either it should not be const, or it should return a new buffer and leave the original intact (which is a different semantic).

11. **No thread safety** — LRU cache, page table, and disk I/O all need synchronization if used from multiple threads. Not necessarily required now, but should be documented.

### Minor

12. **`TFilePath` for pageFiles vs `std::filesystem::path`** — HSTypes defines `TFilePath` as `std::filesystem::path`. Consistent use is good, but `GetPageFilePath` returns `TFilePath` while the member is also `TFilePath` — redundant aliasing.

13. **`buffer` member name shadows concept** — `buffer` is a `std::vector<PageCache>` but the field name suggests a flat buffer. `pageCache` or `cachedPages` would be clearer.

14. **No page size constant** — page length is determined dynamically. A `kPageSize` constant (e.g., 1000 lines) would make the design clearer and allow tuning.

15. **`LoadPageFromDisk` signature mismatch** — declared as `void LoadPageFromDisk(TPageIndex pageIndex, TLineIndex startLine) const` but the `const` qualifier is wrong if it loads data into the non-const `buffer` vector.

---

## Plan

### Phase 1: Foundation (Core Data Structures)

| Step | Task | Effort |
|------|------|--------|
| 1.1 | Add `IsModified` flag to `PageCache` | Small |
| 1.2 | Add `kDefaultPageSize` constant (1000 lines) | Small |
| 1.3 | Rename `buffer` → `cachedPages`, make page table private with accessors | Small |
| 1.4 | Fix `LoadPageFromDisk` const-correctness | Small |

### Phase 2: Core Routing (FindPage + GetLine)

| Step | Task | Effort |
|------|------|--------|
| 2.1 | Implement `FindPage(lineNumber)` — binary search over `pageLineStart` | Medium |
| 2.2 | Implement `GetLine(lineNumber)` — find page, ensure loaded, return line | Medium |
| 2.3 | Implement `NumLines()` — sum of pageLengths (or cached for O(1)) | Small |
| 2.4 | Implement `NumPages()` — `pageLength.size()` | Trivial |
| 2.5 | Implement `NumLoadedPages()` — `cachedPages.size()` | Trivial |

### Phase 3: LRU Cache

| Step | Task | Effort |
|------|------|--------|
| 3.1 | Keep `std::vector<PageCache>` — O(n) shifts negligible for bounded caches | Trivial |
| 3.2 | Add `std::unordered_map<TPageIndex, size_t>` for O(1) `IsPageLoaded` lookup | Small |
| 3.3 | Implement `Touch(pageIndex)` — swap to back of vector (MRU position) | Small |
| 3.4 | Implement `EvictLRU()` — pop front if at capacity | Small |
| 3.5 | Implement `EnsurePageLoaded(pageId)` — load if missing, evict if needed | Medium |
| 3.6 | Implement `UnloadPage(pageId)` — write back if dirty, remove from cache | Medium |

### Phase 4: Mutation Operations

| Step | Task | Effort |
|------|------|--------|
| 4.1 | Implement `AddLine(line)` — append to last page or create new page | Medium |
| 4.2 | Implement `InsertLine(lineNo, line)` — find page, insert, adjust indices | Large |
| 4.3 | Implement `ReplaceLine(lineNo, line)` — find page, replace | Medium |
| 4.4 | Implement `RemoveLine(lineNo)` — find page, remove | Medium |
| 4.5 | Implement `ExtractLine(lineNo)` — RemoveLine + return value | Small |

### Phase 5: Persistence

| Step | Task | Effort |
|------|------|--------|
| 5.1 | Implement `GetPageFilePath(pageId)` — `<dir>/page_<id>.txt` | Small |
| 5.2 | Implement `SavePageToDisk(pageId)` — write TextBuffer to file | Medium |
| 5.3 | Implement `Open()` — load base file, split into pages, populate page table | Large |
| 5.4 | Implement `Close()` — flush all dirty pages, optionally merge | Large |

### Phase 6: Advanced Operations

| Step | Task | Effort |
|------|------|--------|
| 6.1 | Implement `Split(splitLine)` — return new buffer for lines after splitLine | Large |
| 6.2 | Set `SetMaxPages()` — update capacity and evict if needed | Small |
| 6.3 | Add thread safety (mutex) if multi-threaded access is expected | Medium |

### Phase 7: Verification

| Step | Task | Effort |
|------|------|--------|
| 7.1 | Write unit tests for FindPage, GetLine, AddLine, InsertLine, RemoveLine | Large |
| 7.2 | Test LRU eviction under memory pressure | Medium |
| 7.3 | Test persistence roundtrip (Open → Modify → Close → Open) | Large |
| 7.4 | Benchmark against sequential access patterns | Medium |
