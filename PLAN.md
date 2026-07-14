# HS Edit — Implementation Plan

## Overview

HS Edit is a minimal core editor that delegates everything to modules. The core provides:
- ncurses initialization/shutdown
- Module discovery and registration
- Input capture and command dispatch
- Rendering infrastructure (windows, buffers)

Modules provide all functionality: editing, navigation, file I/O, search, etc.

## Categories

### 1. Project Foundation
Setup project infrastructure: directory structure, build system, git workflows.

### 2. Core Systems
Infrastructure that other components depend on: module system, test framework, protocol.

### 3. Core Editor
Minimal editor: input capture, command dispatch, rendering infrastructure, window/buffer management.

### 4. Basic Modules
Feature modules that provide editing functionality: file I/O, navigation, editing, search, undo/redo, config.

### 5. Polish & Documentation
Performance optimization, testing, documentation, CI/CD.

---

## Tasks

### 1. Project Foundation

- [ ] Create directory structure (`src/`, `modules/`, `pages/`, `docs/`, `tests/`)
- [ ] Create `CMakeLists.txt` (C++17, ncurses linking)
- [ ] Create `.gitignore`
- [ ] Verify build system works (`cmake ..`, `cmake --build .`)

**Verification:**
- [ ] Project compiles without errors
- [ ] Executable runs (displays nothing or "Hello")
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

### 3. Core Editor

#### 3.1 ncurses Initialization
- [ ] Initialize ncurses (`initscr`, `cbreak`, `noecho`, `keypad`)
- [ ] Handle terminal resize (`KEY_RESIZE`)
- [ ] Restore terminal on exit (`endwin`)

**Verification:**
- [ ] Terminal is in raw mode
- [ ] Special keys (arrows, F-keys) are captured
- [ ] Terminal is restored on exit

#### 3.2 Input Capture
- [ ] Capture keystrokes (`getch`)
- [ ] Parse key events (regular keys, special keys, modifiers)
- [ ] Implement event bus (publish key events)

**Verification:**
- [ ] All key combinations are captured
- [ ] Events are published correctly
- [ ] Modifier keys (Ctrl, Alt) work

#### 3.3 Command Dispatch
- [ ] Implement command router (key → command → module)
- [ ] Implement module capability lookup
- [ ] Implement command execution (spawn module, send command, read response)
- [ ] Implement `[call]` callback processing

**Verification:**
- [ ] Key bindings trigger correct modules
- [ ] Commands execute and return results
- [ ] Callbacks update editor state

#### 3.4 Rendering Infrastructure
- [ ] Implement screen buffer (double buffering)
- [ ] Implement window management (create, destroy, resize)
- [ ] Implement buffer management (create, destroy, associate with window)
- [ ] Implement text drawing primitives (`draw_char`, `draw_line`, `draw_text`)
- [ ] Implement refresh (draw screen buffer to terminal)

**Verification:**
- [ ] Can draw text at any position
- [ ] Multiple windows display correctly
- [ ] Screen refreshes without flicker

#### 3.5 Window/Buffer Management
- [ ] Implement `Window` struct (viewport)
- [ ] Implement `Buffer` struct (text content)
- [ ] Implement window-buffer association
- [ ] Implement window switching
- [ ] Implement buffer creation/destruction

**Verification:**
- [ ] Windows can show different buffers
- [ ] Can switch window to different buffer
- [ ] Buffers persist when no windows reference them

---

### 4. Basic Modules

#### 4.1 Navigation Module
- [ ] Handle arrow keys (up, down, left, right)
- [ ] Handle home/end
- [ ] Handle page up/down
- [ ] Handle word navigation (Ctrl+left, Ctrl+right)
- [ ] Register key bindings (`KEY_UP`, `KEY_DOWN`, etc.)

**Verification:**
- [ ] Arrow keys move cursor correctly
- [ ] Home/end work
- [ ] Page up/down work

#### 4.2 Editing Module
- [ ] Handle character insertion (printable keys)
- [ ] Handle backspace
- [ ] Handle delete
- [ ] Handle Enter (line break)
- [ ] Handle tab (insert spaces or tab character)

**Verification:**
- [ ] Can type text and see it on screen
- [ ] Backspace/delete work correctly
- [ ] Enter creates new line
- [ ] Tab inserts correct whitespace

#### 4.3 File Manager Module
- [ ] Implement `:open <path>` command
- [ ] Implement `:save` command
- [ ] Implement `:saveas <path>` command
- [ ] Implement `:new` command (empty buffer)
- [ ] Handle file not found errors
- [ ] Handle permission errors

**Verification:**
- [ ] Can open any text file
- [ ] Can save edits to file
- [ ] Can save with different filename
- [ ] Error messages are clear

#### 4.4 Undo/Redo Module
- [ ] Implement undo stack
- [ ] Implement redo stack
- [ ] Handle `:undo` command
- [ ] Handle `:redo` command
- [ ] Handle undo on line breaks, insertions, deletions
- [ ] Limit undo stack size (e.g., 1000 operations)

**Verification:**
- [ ] Undo reverses last edit
- [ ] Redo re-applies undone edit
- [ ] Undo/redo works across line breaks

#### 4.5 Search Module
- [ ] Handle `:search <pattern>` command
- [ ] Handle `:searchb <pattern>` command (backward)
- [ ] Handle `:next` command (next match)
- [ ] Handle `:prev` command (previous match)
- [ ] Highlight search matches
- [ ] Case-sensitive/insensitive option

**Verification:**
- [ ] Search finds correct matches
- [ ] Next/prev moves between matches
- [ ] Search highlights are visible

#### 4.6 Replace Module
- [ ] Handle `:replace <old> <new>` command (single)
- [ ] Handle `:replaceall <old> <new>` command
- [ ] Confirm before replace (optional)
- [ ] Show count of replacements

**Verification:**
- [ ] Replace works correctly
- [ ] Replace all updates all occurrences

#### 4.7 Config Module
- [ ] Handle `:set key value` command
- [ ] Handle `:get key` command
- [ ] Handle `:unset key` command
- [ ] Save config to `config.txt`
- [ ] Load config on startup

**Verification:**
- [ ] Can set/get/unset config values
- [ ] Config persists across restarts
- [ ] Validations work (e.g., tab_width must be int)

#### 4.8 Command Mode Module
- [ ] Handle `:` key (enter command mode)
- [ ] Show command input line at bottom
- [ ] Execute commands (route to appropriate module)
- [ ] Exit command mode on Enter or Escape
- [ ] Show command history (up/down arrows)

**Verification:**
- [ ] `:` activates command mode
- [ ] Commands execute correctly
- [ ] Can exit command mode
- [ ] History works

---

### 5. Polish & Documentation

#### 5.1 Performance Optimization
- [ ] Profile editor (identify bottlenecks)
- [ ] Optimize rendering (only redraw changed regions)
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
| 2. Core Systems | Week 2 | 📋 Planned |
| 3. Core Editor | Week 3-4 | 📋 Planned |
| 4. Basic Modules | Week 5-8 | 📋 Planned |
| 5. Polish & Documentation | Week 9-10 | 📋 Planned |

## Next Steps

1. **Start Category 1** — Project Foundation (CMakeLists.txt, directory structure)
2. **Then Category 2** — Core Systems (module system, test framework)
3. **Then Category 3** — Core Editor (minimal: input, render, dispatch)
4. **Then Category 4** — Basic Modules (navigation, editing, file I/O, etc.)
5. **Finally Category 5** — Polish & Documentation

## Resources

- [Technical Design](TECHNICAL_DESIGN.md)
- [NCurses Tutorial](docs/ncurses-tutorial.html)
- [Setup Scripts](scripts/)
