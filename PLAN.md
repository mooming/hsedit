# HS Edit — Implementation Plan

## Overview

This plan outlines the implementation phases for HS Edit, a modular TUI text editor. The editor follows a microservices architecture with modules as independent executables communicating via standard I/O.

## Phases

### Phase 1: Project Foundation (Week 1-2)

**Goal:** Set up project structure, build system, and basic editor framework.

#### 1.1 Project Structure
- [ ] Create directory layout (`src/`, `modules/`, `pages/`, `docs/`)
- [ ] Create `CMakeLists.txt` with:
  - C++17 standard
  - ncurses linking
  - Module build targets
- [ ] Create `.gitignore`

**Verification:**
- [ ] `cmake ..` succeeds
- [ ] `cmake --build .` produces executable
- [ ] Directory structure matches specification

#### 1.2 Core Data Structures
- [ ] Implement `ProtocolMessage` struct
- [ ] Implement `ModuleInfo` struct
- [ ] Implement `KeyBinding` struct
- [ ] Implement `Page` struct
- [ ] Implement `PageTable` class
- [ ] Implement `Buffer` struct
- [ ] Implement `Window` struct

**Verification:**
- [ ] All structs compile without errors
- [ ] Unit tests for basic struct operations (if applicable)

#### 1.3 Module Discovery System
- [ ] Implement `ModuleRegistry` class
- [ ] Implement directory scanning for `modules/`
- [ ] Implement `--reveal` command handler
- [ ] Implement registration parsing

**Verification:**
- [ ] Test module discovery with sample modules
- [ ] Test `--reveal` command response parsing
- [ ] Unit tests for ModuleRegistry

#### 1.4 Protocol Implementation
- [ ] Implement line-based message parsing
- [ ] Implement `[call]` callback parser
- [ ] Implement message serialization
- [ ] Implement protocol constants

**Verification:**
- [ ] Unit tests for message parsing
- [ ] Unit tests for callback parsing
- [ ] Round-trip test (serialize → parse → compare)

#### 1.5 Basic Editor Framework
- [ ] Implement `Editor` main class
- [ ] Implement `InputHandler` (ncurses integration)
- [ ] Implement `RenderEngine` (basic rendering)
- [ ] Implement `Router` (command dispatch)

**Verification:**
- [ ] Editor starts and displays basic screen
- [ ] Key events are captured
- [ ] Basic text can be rendered

**Commit:** `Phase 1: Project Foundation`

---

### Phase 2: Core Editor Features (Week 3-4)

**Goal:** Implement core editor functionality (input, rendering, basic editing).

#### 2.1 Input Handling
- [ ] Implement key event parsing (regular keys, special keys, modifiers)
- [ ] Implement keybinding system (map key → command)
- [ ] Implement command mode (colon key → command input)

**Verification:**
- [ ] All key combinations work (arrows, F-keys, Ctrl combos)
- [ ] Command mode activates with `:`
- [ ] Commands can be executed from command line

#### 2.2 Rendering Engine
- [ ] Implement screen buffer (double buffering)
- [ ] Implement window management (create, destroy, resize)
- [ ] Implement text rendering (lines, colors, attributes)
- [ ] Implement status bar

**Verification:**
- [ ] Text renders correctly at any position
- [ ] Multiple windows display correctly
- [ ] Colors and attributes work
- [ ] Status bar shows cursor position

#### 2.3 Basic Editing
- [ ] Implement character insertion
- [ ] Implement character deletion (backspace, delete)
- [ ] Implement line break (Enter key)
- [ ] Implement cursor movement (arrow keys, home, end)

**Verification:**
- [ ] Can type text and see it on screen
- [ ] Backspace/delete work correctly
- [ ] Enter creates new line
- [ ] Cursor moves correctly with all navigation keys

#### 2.4 Page-Based Text Storage
- [ ] Implement `PageTable::find_page()`
- [ ] Implement `PageTable::split_page()`
- [ ] Implement page loading (from disk)
- [ ] Implement page saving (to disk)

**Verification:**
- [ ] Text persists across editor restarts
- [ ] Pages split correctly when growing
- [ ] Large files (>10MB) work without memory issues

**Commit:** `Phase 2: Core Editor Features`

---

### Phase 3: Module System (Week 5-6)

**Goal:** Implement full module system with IPC and on-demand execution.

#### 3.1 Module Execution
- [ ] Implement process spawning (fork/exec)
- [ ] Implement pipe setup (stdin/stdout)
- [ ] Implement command sending
- [ ] Implement response reading
- [ ] Implement process cleanup

**Verification:**
- [ ] Modules spawn and execute correctly
- [ ] Commands are sent and responses received
- [ ] Processes are cleaned up after execution
- [ ] Error handling for module failures

#### 3.2 Command Dispatch
- [ ] Implement command lookup (key → command → module)
- [ ] Implement command routing (router → module)
- [ ] Implement `[call]` callback processing

**Verification:**
- [ ] Key bindings trigger correct modules
- [ ] Commands execute and return results
- [ ] Callbacks update editor state

#### 3.3 Base Modules
- [ ] **Command Mode Module**
  - [ ] Handle `:` key
  - [ ] Show command input line
  - [ ] Execute commands (e.g., `:q`, `:w`)
  
- [ ] **Navigation Module**
  - [ ] Handle arrow keys
  - [ ] Handle home/end
  - [ ] Handle page up/down
  
- [ ] **File Manager Module**
  - [ ] Handle `:open <path>`
  - [ ] Handle `:save`
  - [ ] Handle `:quit`

**Verification:**
- [ ] Command mode works (`:` → input line → execute)
- [ ] Navigation keys move cursor correctly
- [ ] Can open, edit, and save files
- [ ] Can quit editor with `:q`

**Commit:** `Phase 3: Module System`

---

### Phase 4: Advanced Features (Week 7-8)

**Goal:** Implement advanced features (multi-window, config, search).

#### 4.1 Multi-Window Support
- [ ] Implement window splitting (horizontal, vertical)
- [ ] Implement window switching
- [ ] Implement buffer-window association
- [ ] Implement window resizing

**Verification:**
- [ ] Can split screen into multiple windows
- [ ] Each window shows different buffer
- [ ] Can switch focus between windows
- [ ] Resizing works correctly

#### 4.2 Config System
- [ ] Implement config file parsing (`config.txt`)
- [ ] Implement typed getters (int, bool, string)
- [ ] Implement config hot-reload
- [ ] Implement module config extension

**Verification:**
- [ ] Config values load correctly
- [ ] Typed getters return correct types
- [ ] Config changes apply without restart

#### 4.3 Search and Replace
- [ ] Implement forward search
- [ ] Implement backward search
- [ ] Implement replace (single)
- [ ] Implement replace all

**Verification:**
- [ ] Search finds correct matches
- [ ] Replace works correctly
- [ ] Replace all updates all occurrences

#### 4.4 Undo/Redo
- [ ] Implement undo stack
- [ ] Implement redo stack
- [ ] Implement undo command
- [ ] Implement redo command

**Verification:**
- [ ] Undo reverses last edit
- [ ] Redo re-applies undone edit
- [ ] Undo/redo works across line breaks

**Commit:** `Phase 4: Advanced Features`

---

### Phase 5: Polish and Documentation (Week 9-10)

**Goal:** Optimize performance, write documentation, add tests.

#### 5.1 Performance Optimization
- [ ] Profile editor (identify bottlenecks)
- [ ] Optimize rendering (only redraw changed regions)
- [ ] Optimize page loading (LRU cache)
- [ ] Optimize module communication (batch commands)

**Verification:**
- [ ] Editor responds to input within 50ms
- [ ] Large files (>10MB) open within 2 seconds
- [ ] Memory usage stays below 50MB for typical use

#### 5.2 Testing
- [ ] Unit tests for core data structures
- [ ] Unit tests for protocol parsing
- [ ] Integration tests for module system
- [ ] End-to-end tests for editing workflow

**Verification:**
- [ ] All tests pass
- [ ] Test coverage > 80%
- [ ] CI/CD pipeline runs tests on PR

#### 5.3 Documentation
- [ ] Write module development guide
- [ ] Write user manual
- [ ] Add inline code documentation
- [ ] Update TECHNICAL_DESIGN.md with implementation notes

**Verification:**
- [ ] Module guide is clear and complete
- [ ] User manual covers all features
- [ ] Code is documented with doxygen comments

**Commit:** `Phase 5: Polish and Documentation`

---

## Timeline

| Phase | Duration | Status |
|-------|----------|--------|
| Phase 1: Project Foundation | Week 1-2 | 🚧 In Progress |
| Phase 2: Core Editor Features | Week 3-4 | 📋 Planned |
| Phase 3: Module System | Week 5-6 | 📋 Planned |
| Phase 4: Advanced Features | Week 7-8 | 📋 Planned |
| Phase 5: Polish and Documentation | Week 9-10 | 📋 Planned |

## Next Steps

1. **Start Phase 1.1** — Create project structure and CMakeLists.txt
2. **Implement core data structures** — ProtocolMessage, ModuleInfo, Page, Buffer, Window
3. **Implement module discovery** — Directory scanning and --reveal command

## Resources

- [Technical Design](TECHNICAL_DESIGN.md)
- [NCurses Tutorial](docs/ncurses-tutorial.html)
- [Setup Scripts](scripts/)
