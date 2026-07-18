# VirtualTextBuffer Test Plan

## Test Categories

### 1. Page Routing (`FindPage`)

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| RT-01 | Find page for line 0 | lineNumber=0 | pageId=0 |
| RT-02 | Find page for last line of first page | lineNumber=kPageSize-1 | pageId=0 |
| RT-03 | Find page for first line of second page | lineNumber=kPageSize | pageId=1 |
| RT-04 | Find page for middle of third page | lineNumber=kPageSize*2 + kPageSize/2 | pageId=2 |
| RT-05 | Find page for last line of last page | lineNumber=totalLines-1 | pageId=numPages-1 |
| RT-06 | Find page with empty buffer | lineNumber=0 | pageId=0 (or ERR) |

### 2. Line Access (`GetLine`)

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| LA-01 | Get line from loaded page | lineNumber=5 | Returns line content |
| LA-02 | Get line triggers page load | lineNumber=0 (page not loaded) | Page loaded, returns line |
| LA-03 | GetLine out of bounds | lineNumber=NumLines() | Throws or returns error |
| LA-04 | GetLine after RemoveLine | lineNumber=0 (removed) | Throws or returns error |

### 3. LRU Cache Behavior

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| LRU-01 | Load page when cache empty | EnsurePageLoaded(0) | Page loaded, NumLoadedPages=1 |
| LRU-02 | Load page when cache has space | EnsurePageLoaded(5) with capacity=10 | Page loaded, NumLoadedPages=1 |
| LRU-03 | Evict LRU when at capacity | Load pages 0-9, then load page 10 | Page 0 evicted, pages 1-10 in cache |
| LRU-04 | Touch promotes to MRU | Load pages 0,1,2. Touch page 0. Evict. | Page 1 evicted (not 0) |
| LRU-05 | IsPageLoaded returns correct status | EnsurePageLoaded(0). IsPageLoaded(0) | true |
| LRU-06 | IsPageLoaded returns false for unloaded | EnsurePageLoaded(0). IsPageLoaded(1) | false |
| LRU-07 | Unlimited cache (maxPages=0) | Load 100 pages with maxPages=0 | All 100 pages remain loaded |

### 4. Mutation Operations

#### 4.1 AddLine

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| AL-01 | Add line to empty buffer | AddLine("test") | NumLines=1, line 0="test" |
| AL-02 | Add line to non-empty buffer | NumLines=5. AddLine("new") | NumLines=6, line 5="new" |
| AL-03 | AddLine triggers new page | AddLine to last line of page | New page created, NumPages=2 |
| AL-04 | AddLine marks page dirty | AddLine("test"). SaveAll. Open again. | Line persists |

#### 4.2 InsertLine

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| IL-01 | Insert at beginning | InsertLine(0, "new"). GetLine(0) | "new" |
| IL-02 | Insert at end | InsertLine(NumLines, "new") | Line added at end |
| IL-03 | Insert in middle | InsertLine(5, "new"). GetLine(5) | "new". Original line 5 becomes 6 |
| IL-04 | Insert crosses page boundary | Insert at last line of page | Page split, lines distributed correctly |
| IL-05 | Insert out of bounds | InsertLine(NumLines+1, "test") | Throws or error |

#### 4.3 ReplaceLine

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| RL-01 | Replace existing line | ReplaceLine(0, "new"). GetLine(0) | "new" |
| RL-02 | Replace last line | ReplaceLine(NumLines-1, "new") | Last line updated |
| RL-03 | Replace out of bounds | ReplaceLine(NumLines, "test") | Throws or error |

#### 4.4 RemoveLine

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| RM-01 | Remove first line | RemoveLine(0). NumLines() | NumLines-1 |
| RM-02 | Remove last line | RemoveLine(NumLines-1). NumLines() | NumLines-1 |
| RM-03 | Remove middle line | RemoveLine(5). GetLine(5) | Original line 6 |
| RM-04 | Remove out of bounds | RemoveLine(NumLines) | Throws or error |
| RM-05 | Remove triggers page merge | Remove from boundary, pages become empty | Pages merged if both empty |

#### 4.5 ExtractLine

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| EL-01 | Extract returns line content | ExtractLine(0) | Returns "test" |
| EL-02 | Extract removes line | ExtractLine(0). NumLines() | NumLines-1 |
| EL-03 | Extract out of bounds | ExtractLine(NumLines) | Throws or error |

### 5. Persistence

#### 5.1 Save/Load Roundtrip

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| PS-01 | Save empty buffer | Create empty. SaveAll. Open. | NumLines=0 |
| PS-02 | Save single page | Add 100 lines. SaveAll. Open. | All 100 lines present |
| PS-03 | Save multi-page | Add 2500 lines. SaveAll. Open. | All 2500 lines present |
| PS-04 | Save after modification | Add 100 lines. ReplaceLine(50, "modified"). SaveAll. Open. | Line 50="modified", others unchanged |
| PS-05 | Save after insertion | Add 100 lines. InsertLine(50, "inserted"). SaveAll. Open. | Line 50="inserted", original 50+ shifted |
| PS-06 | Save after removal | Add 100 lines. RemoveLine(50). SaveAll. Open. | 99 lines, original 51+ shifted to 50+ |

#### 5.2 Page File Generation

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| PF-01 | Page files created on Open | Open(file). NumPages()=3 | 3 page files exist on disk |
| PF-02 | Page file naming | Open(file). GetPageFilePath(0) | "<dir>/page_0.txt" |
| PF-03 | Page file contains correct data | Open(file). Read page 0 file | Contains first kPageSize lines |

### 6. Split Operation

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| SP-01 | Split at page boundary | Split(kPageSize). Original NumLines, New NumLines | Original=kPageSize, New=remaining |
| SP-02 | Split in middle of page | Split(kPageSize/2). Both NumLines | Both correct, lines distributed |
| SP-03 | Split at end | Split(NumLines). Original, New NumLines | Original=NumLines, New=0 |
| SP-04 | Split at beginning | Split(0). Original, New NumLines | Original=0, New=NumLines |
| SP-05 | Split preserves content | Split(kPageSize/2). GetLine from both | All lines present in correct order |

### 7. Edge Cases

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| EC-01 | Empty buffer operations | NumLines on empty | 0 |
| EC-02 | Single line buffer | AddLine("test"). GetLine(0) | "test" |
| EC-03 | Single page buffer | Add kPageSize lines | NumPages=1 |
| EC-04 | Exact page boundary | Add kPageSize lines. Add 1 more | NumPages=2 |
| EC-05 | Large buffer | Add 100000 lines | All accessible, no OOM |
| EC-06 | Rapid load/unload | Loop: EnsurePageLoaded(i). UnloadPage(i) | No leaks, consistent state |

### 8. Integration Tests

| Test ID | Description | Input | Expected Output |
|---------|-------------|-------|-----------------|
| IT-01 | Open → Modify → Close → Open | Open. AddLine. Close. Open. GetLine | Line persists |
| IT-02 | Concurrent read/write simulation | Multiple threads reading/writing | No crashes, consistent state |
| IT-03 | LRU with mutations | Load pages, modify, evict, reload | Modified data persists on reload |
| IT-04 | Split then merge | Split. Modify both halves. Close both. | Data integrity maintained |

## Test Implementation Notes

### Test Framework
- Use Google Test (gtest) for C++ testing
- Tests go in `tests/VirtualTextBuffer_test.cpp`
- Mock file system with temporary directory

### Setup/Teardown
```cpp
class VirtualTextBufferTest : public ::testing::Test {
protected:
    std::string tempDir;
    
    void SetUp() override {
        tempDir = CreateTempDirectory();
    }
    
    void TearDown() override {
        DeleteDirectory(tempDir);
    }
};
```

### Helper Functions
```cpp
// Create buffer with N lines
std::unique_ptr<VirtualTextBuffer> CreateBufferWithLines(size_t numLines);

// Verify all lines match expected
void VerifyLines(const VirtualTextBuffer& buffer, const std::vector<TLine>& expected);

// Get page file path
TFilePath GetPagePath(const std::filesystem::path& dir, TPageIndex pageId);
```

### Key Constants
```cpp
constexpr size_t kDefaultPageSize = 1000;
constexpr size_t kTestCacheSize = 10;
```

## Verification Criteria

- [ ] All 50+ test cases pass
- [ ] No memory leaks (valgrind clean)
- [ ] No data corruption on persistence roundtrip
- [ ] LRU eviction follows correct policy (MRU promoted, LRU evicted)
- [ ] Page boundaries handled correctly for all operations
- [ ] Thread safety verified (if multi-threaded)
