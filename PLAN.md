# HS Edit — Implementation Plan

## Overview

HS Edit is a **minimal core editor** that delegates everything to modules. The core provides:
- Module discovery and registration
- Command dispatch (router)
- Event bus

Modules provide ALL functionality:
- **Terminal Module**: ncurses initialization, input capture, rendering
- **Navigation Module**: cursor movement
- **Editing Module**: character insertion/deletion
- **File Manager Module**: file I/O
- **Search Module**: search/replace
- **Undo Module**: undo/redo
- **Config Module**: configuration
- **Command Mode Module**: `:` command line
- And more...

The core editor has **zero knowledge of ncurses**. It just dispatches commands between modules.

## Categories

### 1. Project Foundation
Setup project infrastructure: directory structure, build system, git workflows.

### 2. Core Systems
Infrastructure that other components depend on: module system, test framework, protocol.

### 3. Core Editor
Minimal editor: module discovery, command dispatch, event bus. No ncurses knowledge.

### 4. Terminal Module
Module that wraps ncurses: initialization, input capture, rendering.

### 5. Basic Modules
Feature modules: navigation, editing, file I/O, search, undo/redo, config, command mode.

### 6. Polish & Documentation
Performance optimization, testing, documentation, CI/CD.

---

## Tasks

### 1. Project Foundation

- [ ] Create directory structure (`src/`, `modules/terminal`, `modules/navigation`, ..., `docs/`, `tests/`)
- [ ] Create `CMakeLists.txt` (C++17, ncurses linking for Terminal Module only)
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

The core editor is a **pure dispatcher**:
1. Scan `modules/`
2. Load Terminal Module (which init ncurses)
3. Load other modules
4. Main loop: ask Terminal Module for input → dispatch → modules respond → loop

**The core editor has ZERO knowledge of ncurses.**

#### 3.1 Module Discovery
- [ ] Scan `modules/` directory for executables
- [ ] Run `module --reveal` for each executable
- [ ] Parse registration response
- [ ] Store module capabilities in registry

**Verification:**
- [ ] All modules in `modules/` are discovered
- [ ] `--reveal` responses are parsed correctly
- [ ] Module capabilities are stored correctly

#### 3.2 Command Dispatch
- [ ] Implement command router (key → command → module)
- [ ] Implement module capability lookup
- [ ] Implement command execution (spawn module, send command, read response)
- [ ] Implement `[call]` callback processing

**Verification:**
- [ ] Key bindings trigger correct modules
- [ ] Commands execute and return results
- [ ] Callbacks update editor state

#### 3.3 Event Bus
- [ ] Implement event publisher/subscriber
- [ ] Implement core events: `ModuleLoaded`, `ModuleUnloaded`, `EditorStarted`, `EditorStopped`
- [ ] Modules can subscribe to events

**Verification:**
- [ ] Events are published on correct operations
- [ ] Modules receive events they subscribed to

---

### 4. Terminal Module

This module wraps ncurses and provides:
- ncurses initialization/shutdown
- Input capture (keystrokes)
- Rendering (drawing into screen buffer)

#### 4.1 ncurses Initialization
- [ ] Initialize ncurses (`initscr`, `cbreak`, `noecho`, `keypad`)
- [ ] Handle terminal resize (`KEY_RESIZE`)
- [ ] Restore terminal on exit (`endwin`)

**Verification:**
- [ ] Terminal is in raw mode
- [ ] Special keys (arrows, F-keys) are captured
- [ ] Terminal is restored on exit

#### 4.2 Input Capture
- [ ] Implement `get_input()` command (calls `getch()`, returns key code)
- [ ] Publish `InputReceived` event with key code

**Verification:**
- [ ] `get_input()` returns correct key codes
- [ ] Events are published correctly

#### 4.3 Rendering
- [ ] Implement `draw_char(y, x, ch, color, attr)` command
- [ ] Implement `draw_text(y, x, text, color, attr)` command
- [ ] Implement `clear()` command
- [ ] Implement `refresh()` command (pushes screen buffer to terminal)

**Verification:**
- [ ] Can draw characters at any position
- [ ] Colors and attributes work
- [ ] `refresh()` pushes buffer to terminal without flicker

---

### 5. Basic Modules

#### 5.1 Navigation Module
- [ ] Handle arrow keys (up, down, left, right)
- [ ] Handle home/end
- [ ] Handle page up/down
- [ ] Handle word navigation (Ctrl+left, Ctrl+right)
- [ ] Register key bindings (`KEY_UP`, `KEY_DOWN`, etc.)

**Verification:**
- [ ] Arrow keys move cursor correctly
- [ ] Home/end work
- [ ] Page up/down work

#### 5.2 Editing Module
- [ ] Handle character insertion (printable keys)
- [ ] Handle backspace
- [ ] Handle delete
- [ ] Handle Enter (line break)
- [ ] Handle tab (insert spaces or tab character)
- [ ] Register key bindings for editing keys

**Verification:**
- [ ] Can type text and see it on screen
- [ ] Backspace/delete work correctly
- [ ] Enter creates new line
- [ ] Tab inserts correct whitespace

#### 5.3 Window Manager Module
- [ ] Handle window creation (`:window new`)
- [ ] Handle window destruction (`:window close`)
- [ ] Handle window splitting (`:split`, `:vsplit`)
- [ ] Handle window switching (`:window next`, `:window prev`)
- [ ] Handle buffer-window association
- [ ] Handle window resizing

**Verification:**
- [ ] Can create/destroy windows
- [ ] Can split screen into multiple windows
- [ ] Can switch between windows
- [ ] Windows show different buffers

#### 5.4 File Manager Module
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

#### 5.5 Undo Module
- [ ] Handle `:undo` command
- [ ] Handle `:redo` command
- [ ] Maintain undo/redo stacks
- [ ] Limit stack size (e.g., 1000 operations)

**Verification:**
- [ ] Undo reverses last edit
- [ ] Redo re-applies undone edit
- [ ] Undo/redo works across line breaks

#### 5.6 Search Module
- [ ] Handle `:search <pattern>` command
- [ ] Handle `:searchb <pattern>` command (backward)
- [ ] Handle `:next` command (next match)
- [ ] Handle `:prev` command (previous match)
- [ ] Notify Terminal Module to highlight matches

**Verification:**
- [ ] Search finds correct matches
- [ ] Next/prev moves between matches
- [ ] Search highlights are visible

#### 5.7 Replace Module
- [ ] Handle `:replace <old> <new>` command (single)
- [ ] Handle `:replaceall <old> <new>` command
- [ ] Notify Undo Module to record changes
- [ ] Show count of replacements

**Verification:**
- [ ] Replace works correctly
- [ ] Replace all updates all occurrences

#### 5.8 Config Module
- [ ] Handle `:set key value` command
- [ ] Handle `:get key` command
- [ ] Handle `:unset key` command
- [ ] Save config to `config.txt`
- [ ] Load config on startup

**Verification:**
- [ ] Can set/get/unset config values
- [ ] Config persists across restarts
- [ ] Validations work (e.g., tab_width must be int)

#### 5.9 Command Mode Module
- [ ] Handle `:` key (enter command mode)
- [ ] Draw command input line at bottom (via Terminal Module)
- [ ] Execute commands (route to appropriate module)
- [ ] Exit command mode on Enter or Escape
- [ ] Show command history (up/down arrows)

**Verification:**
- [ ] `:` activates command mode
- [ ] Commands execute correctly
- [ ] Can exit command mode
- [ ] History works

---

### 6. Polish & Documentation

#### 6.1 Performance Optimization
- [ ] Profile editor (identify bottlenecks)
- [ ] Optimize rendering (only redraw changed regions)
- [ ] Optimize module communication (batch commands)
- [ ] Optimize search (compile regex, use efficient algorithms)

**Verification:**
- [ ] Editor responds to input within 50ms
- [ ] Large files (>10MB) open within 2 seconds
- [ ] Memory usage stays below 50MB for typical use

#### 6.2 Testing
- [ ] Unit tests for core data structures
- [ ] Unit tests for protocol parsing
- [ ] Integration tests for module system
- [ ] End-to-end tests for editing workflow
- [ ] Set up CI/CD (GitHub Actions)

**Verification:**
- [ ] All tests pass
- [ ] Test coverage > 80%
- [ ] CI/CD pipeline runs tests on PR

#### 6.3 Documentation
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
| 3. Core Editor | Week 3 | 📋 Planned |
| 4. Terminal Module | Week 4 | 📋 Planned |
| 5. Basic Modules | Week 5-8 | 📋 Planned |
| 6. Polish & Documentation | Week 9-10 | 📋 Planned |

## Next Steps

1. **Start Category 1** — Project Foundation (CMakeLists.txt, directory structure)
2. **Then Category 2** — Core Systems (module system, test framework)
3. **Then Category 3** — Core Editor (minimal: discovery, dispatch, event bus)
4. **Then Category 4** — Terminal Module (ncurses wrapper)
5. **Then Category 5** — Basic Modules (navigation, editing, file I/O, etc.)
6. **Finally Category 6** — Polish & Documentation

## Resources

- [Technical Design](TECHNICAL_DESIGN.md)
- [NCurses Tutorial](docs/ncurses-tutorial.html)
- [Setup Scripts](scripts/)
