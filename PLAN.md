# HS Edit — Implementation Plan

## Overview

This plan outlines the implementation phases for HS Edit, a modular TUI text editor. The editor follows a microservices architecture with modules as independent executables communicating via standard I/O.

## Categories

### 1. Project Foundation
Setup project infrastructure: directory structure, build system, git workflows.

### 2. Core Systems
Infrastructure that other components depend on: module system, test framework, protocol.

### 3. Basic Functions
Core editor capabilities: input handling, rendering, page-based buffer, event system, KV stores.

### 4. Basic Modules
Feature modules that provide editing functionality: file I/O, navigation, search, undo/redo, config.

### 5. Polish & Documentation
Performance optimization, testing, documentation, CI/CD.

---

## Tasks

### 1. Project Foundation

- [ ] Create directory structure (`src/`, `modules/`, `pages/`, `docs/`, `tests/`)
- [ ] Create `CMakeLists.txt` (C++17, ncurses linking)
- [ ] Create `.gitignore`
- [ ] Set up initial project skeleton (main.cpp, basic includes)
- [ ] Verify build system works (`cmake ..`, `cmake --build .`)

**Verification:**
- [ ] Project compiles without errors
- [ ] Executable runs (displays "Hello" or similar)
- [ ] Directory structure matches specification

---

### 2. Core Systems

#### 2.1 Module System
- [ ] Implement `ModuleRegistry` class
- [ ] Implement directory scanning for `modules/`
- [ ] Implement `--reveal` command handling
- [ ] Implement registration parsing (`[call] register:...`)
- [ ] Implement module spawn/execute/cleanup (IPC)
- [ ] Implement line-based message protocol
- [ ] Implement `[call]` callback parser

**Verification:**
- [ ] Test module discovery with sample modules
- [ ] Test `--reveal` command response parsing
- [ ] Test module spawn and command execution
- [ ] Unit tests for protocol parsing

#### 2.2 Test Framework
- [ ] Set up CTest or Google Test
- [ ] Create test utilities (mock stdin/stdout)
- [ ] Create test helpers for protocol messages

**Verification:**
- [ ] Test framework compiles and runs
- [ ] Sample test passes
- [ ] Protocol parsing tests pass

---

### 3. Basic Functions

#### 3.1 Input Handling
- [ ] Implement key event parsing (regular keys, special keys, modifiers)
- [ ] Implement keybinding system (map key → command)
- [ ] Implement command mode (colon key → command input)
- [ ] Implement event subscription system (modules subscribe to events)

**Verification:**
- [ ] All key combinations work (arrows, F-keys, Ctrl combos)
- [ ] Command mode activates with `:`
- [ ] Commands can be executed from command line
- [ ] Event subscription works (modules receive events)

#### 3.2 Rendering
- [ ] Implement screen buffer (double buffering)
- [ ] Implement window management (create, destroy, resize)
- [ ] Implement text rendering (lines, colors, attributes)
- [ ] Implement status bar
- [ ] Implement cursor rendering

**Verification:**
- [ ] Text renders correctly at any position
- [ ] Multiple windows display correctly
- [ ] Colors and attributes work
- [ ] Status bar shows cursor position and mode

#### 3.3 Page-Based Buffer
- [ ] Implement `Page` struct (vector of lines)
- [ ] Implement `PageTable` class (line range → page mapping)
- [ ] Implement page splitting (when too large)
- [ ] Implement page loading (from disk)
- [ ] Implement page saving (to disk)
- [ ] Implement page merging (when small enough)

**Verification:**
- [ ] Text persists across editor restarts
- [ ] Pages split correctly when growing
- [ ] Pages merge when shrinking
- [ ] Large files (>10MB) work without memory issues

#### 3.4 Function Calls
- [ ] Implement `[call]` callback system (module → editor)
- [ ] Implement `switch_mode` command
- [ ] Implement `move_cursor` command
- [ ] Implement `set_highlights` command
- [ ] Implement `open_buffer` command
- [ ] Implement `close_buffer` command

**Verification:**
- [ ] Modules can call editor functions
- [ ] Callbacks update editor state correctly
- [ ] Error handling for invalid callbacks

#### 3.5 Event Subscription
- [ ] Implement event bus (publish/subscribe)
- [ ] Implement core events: `BufferOpen`, `BufferClose`, `BufferModified`, `CursorMoved`
- [ ] Implement module event handlers

**Verification:**
- [ ] Events are published on correct operations
- [ ] Modules receive events they subscribed to
- [ ] Event ordering is correct

#### 3.6 KV Stores
- [ ] Implement `Config` class (key-value store)
- [ ] Implement config file parsing (`config.txt` with `key = value`)
- [ ] Implement typed getters (int, bool, string)
- [ ] Implement config hot-reload
- [ ] Implement module config extension

**Verification:**
- [ ] Config values load correctly
- [ ] Typed getters return correct types
- [ ] Config changes apply without restart

---

### 4. Basic Modules

#### 4.1 File Load/Save
- [ ] Implement file reading (page-based)
- [ ] Implement file writing (page-based)
- [ ] Implement `:open <path>` command
- [ ] Implement `:save` command
- [ ] Implement `:saveas <path>` command
- [ ] Handle file not found errors
- [ ] Handle permission errors

**Verification:**
- [ ] Can open any text file
- [ ] Can save edits to file
- [ ] Can save with different filename
- [ ] Error messages are clear

#### 4.2 Basic Edits
- [ ] Implement character insertion
- [ ] Implement character deletion (backspace, delete)
- [ ] Implement line break (Enter key)
- [ ] Implement cursor movement (arrow keys, home, end)
- [ ] Implement page up/down
- [ ] Implement word navigation

**Verification:**
- [ ] Can type text and see it on screen
- [ ] Backspace/delete work correctly
- [ ] Enter creates new line
- [ ] Cursor moves correctly with all navigation keys

#### 4.3 Undo/Redo
- [ ] Implement undo stack
- [ ] Implement redo stack
- [ ] Implement undo command (`:undo`)
- [ ] Implement redo command (`:redo`)
- [ ] Limit undo stack size (e.g., 1000 operations)

**Verification:**
- [ ] Undo reverses last edit
- [ ] Redo re-applies undone edit
- [ ] Undo/redo works across line breaks
- [ ] Undo stack doesn't grow unbounded

#### 4.4 Search
- [ ] Implement forward search (`:search <pattern>`)
- [ ] Implement backward search (`:searchb <pattern>`)
- [ ] Implement next match (`:next`)
- [ ] Implement previous match (`:prev`)
- [ ] Highlight search matches
- [ ] Case-sensitive/insensitive option

**Verification:**
- [ ] Search finds correct matches
- [ ] Next/prev moves between matches
- [ ] Search highlights are visible
- [ ] Case options work correctly

#### 4.5 Replace
- [ ] Implement single replace (`:replace <old> <new>`)
- [ ] Implement replace all (`:replaceall <old> <new>`)
- [ ] Confirm before replace (optional)
- [ ] Show count of replacements

**Verification:**
- [ ] Replace works correctly
- [ ] Replace all updates all occurrences
- [ ] Confirmation prompt works

#### 4.6 Config Module
- [ ] Implement `:set key value` command
- [ ] Implement `:get key` command
- [ ] Implement `:unset key` command
- [ ] Save config to `config.txt`

**Verification:**
- [ ] Can set/get/unset config values
- [ ] Config persists across restarts
- [ ] Validations work (e.g., tab_width must be int)

---

### 5. Polish & Documentation

#### 5.1 Performance Optimization
- [ ] Profile editor (identify bottlenecks)
- [ ] Optimize rendering (only redraw changed regions)
- [ ] Optimize page loading (LRU cache)
- [ ] Optimize module communication (batch commands)
- [ ] Optimize search (compile regex, use efficient algorithms)

**Verification:**
- [ ] Editor responds to input within 50ms
- [ ] Large files (>10MB) open within 2 seconds
- [ ] Memory usage stays below 50MB for typical use

#### 5.2 Testing
- [ ] Unit tests for core data structures
- [ ] Unit tests for protocol parsing
- [ ] Integration tests for module system
- [ ] End-to-end tests for editing workflow
- [ ] Set up CI/CD (GitHub Actions)

**Verification:**
- [ ] All tests pass
- [ ] Test coverage > 80%
- [ ] CI/CD pipeline runs tests on PR

#### 5.3 Documentation
- [ ] Write module development guide
- [ ] Write user manual
- [ ] Add inline code documentation (doxygen)
- [ ] Update TECHNICAL_DESIGN.md with implementation notes
- [ ] Update README.md with latest status

**Verification:**
- [ ] Module guide is clear and complete
- [ ] User manual covers all features
- [ ] Code is documented with doxygen comments

---

## Timeline

| Category | Duration | Status |
|----------|----------|--------|
| 1. Project Foundation | Week 1 | 🚧 In Progress |
| 2. Core Systems | Week 2-3 | 📋 Planned |
| 3. Basic Functions | Week 4-6 | 📋 Planned |
| 4. Basic Modules | Week 7-9 | 📋 Planned |
| 5. Polish & Documentation | Week 10-11 | 📋 Planned |

## Next Steps

1. **Start Category 1** — Project Foundation (CMakeLists.txt, directory structure)
2. **Then Category 2** — Core Systems (module system, test framework)
3. **Then Category 3** — Basic Functions (input, rendering, buffer)
4. **Then Category 4** — Basic Modules (file I/O, navigation, search)
5. **Finally Category 5** — Polish & Documentation

## Resources

- [Technical Design](TECHNICAL_DESIGN.md)
- [NCurses Tutorial](docs/ncurses-tutorial.html)
- [Setup Scripts](scripts/)
