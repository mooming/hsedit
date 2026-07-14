# HS Edit — Technical Design

## Overview

HS Edit is a modular TUI text editor built with C++ and ncurses. The editor follows a microservices architecture where core functionality is handled by the base editor, and features are implemented as independent executables (modules) that communicate via standard I/O.

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    HS Edit Editor                       │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Input      │  │  Render     │  │  Router     │    │
│  │  Handler    │  │  Engine     │  │  (Command   │    │
│  │             │  │             │  │   Dispatcher)│    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
│                       │                                │
│  ┌────────────────────┴────────────────────────────┐   │
│  │              Module Registry                     │   │
│  │  (tracks available modules and their capabilities)│   │
│  └─────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
         │                    │                    │
         ▼                    ▼                    ▼
┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
│  Syntax         │  │  File           │  │  Command        │
│  Highlight      │  │  Manager        │  │  Mode           │
│  Module         │  │  Module         │  │  Module         │
└─────────────────┘  └─────────────────┘  └─────────────────┘
```

### Core Components

1. **Input Handler** — Receives keystrokes from ncurses, dispatches to router
2. **Render Engine** — Handles ncurses rendering (windows, buffers, status bar)
3. **Router** — Command dispatcher that determines which module handles each command
4. **Module Registry** — Tracks registered modules and their capabilities

### Module System

Modules are standalone executables that:

1. **Discover**: Located in `modules/` directory
2. **Register**: Called with `--reveal` flag to announce capabilities
3. **Execute**: Spawned on-demand when their command is invoked
4. **Communicate**: Use standard I/O with line-based protocol

#### Module Lifecycle

```
Editor Startup:
  1. Scan modules/ directory
  2. For each executable: run `module --reveal`
  3. Parse registration response
  4. Store in module registry

Command Invocation:
  1. User presses key combination
  2. Router looks up: who handles this key?
  3. Spawn module process
  4. Send command: "COMMAND_NAME\n"
  5. Read response from stdout
  6. Process [call] callbacks if present
  7. Close module process
```

## Protocol Specification

### Module → Editor Communication

Modules communicate with the editor via stdout. Each message is a line terminated by `\n`.

#### Registration (`--reveal`)

```
[call] register: name="syntax-highlight", version="1.0", handles="HIGHLIGHT"
[call] register_key: key="g g", command="NAVIGATE_TO_TOP"
[call] register_capability: capability="SEARCH"
```

#### Command Execution

```
Module -> Editor: COMMAND_NAME\n
Module -> Editor: [call] function_name: param1, param2\n
Module -> Editor: RESULT\n (or ERROR: reason\n)
```

#### Editor Response

```
Editor -> Module: OK\n
Editor -> Module: ERROR: reason\n
```

### Command Examples

**Syntax Highlighting:**
```
Editor -> Module: HIGHLIGHT /path/to/file.cpp\n
Module -> Editor: [call] set_highlights: line=0, start=5, end=10, type=keyword\n
Module -> Editor: [call] set_highlights: line=0, start=15, end=20, type=string\n
Module -> Editor: DONE\n
```

**Navigation:**
```
Editor -> Module: NAVIGATE_TO_TOP\n
Module -> Editor: [call] move_cursor: 0, 0\n
Module -> Editor: DONE\n
```

**File Save:**
```
Editor -> Module: SAVE /path/to/file.cpp\n
Module -> Editor: OK\n
```

## Text Representation: Page-Based Model

### Page Structure

Text is stored in **pages**, which are dynamic buffers containing a vector of lines.

```cpp
struct Page {
    std::vector<std::string> lines;
    size_t page_id;
    size_t start_line;  // First line number in this page
    size_t end_line;    // Last line number in this page (exclusive)
    bool modified;
    
    // Split when page grows too large
    Page split() const;
};
```

### Page Table

A sorted mapping of line ranges to pages:

```cpp
struct PageRange {
    size_t start_line;
    size_t end_line;
    size_t page_id;
    bool loaded;  // Whether page is in memory
};

class PageTable {
    std::vector<PageRange> ranges;  // Sorted by start_line
    
    // Find which page contains line N
    PageRange* find_page(size_t line);
    
    // Split page at line boundary
    void split_page(size_t page_id, size_t split_at);
    
    // Merge adjacent pages if both are small enough
    void try_merge();
};
```

### Page Splitting Strategy

When a page grows beyond a threshold (e.g., 2x initial size), it splits into two:

```
Before:  Page 2: [100-300] (200 lines)
After:   Page 2:  [100-175] (75 lines)
         Page 2a: [176-300] (125 lines)

Page Table updates:
  [100, 300] → Page 2
  becomes:
  [100, 175] → Page 2
  [176, 300] → Page 2a
```

**Split threshold:** 2x initial page size (1000 lines → split at 2000 lines)

### Persistence

Pages are cached to disk as individual files:

```
pages/
├── page_0.dat      # Lines 0-999
├── page_1.dat      # Lines 1000-1999
├── page_2.dat      # Lines 2000-2999
└── ...
```

**Load:** Read page file into memory when needed
**Save:** Write modified pages back to disk
**Rebuild:** On save, iterate all pages and write to single file

## Window/Buffer Model

### Buffer

A buffer represents a text document:

```cpp
struct Buffer {
    size_t buffer_id;
    std::string file_path;
    PageTable page_table;
    bool modified;
    std::unordered_set<size_t> window_refs;  // Windows pointing to this buffer
};
```

### Window

A window is a viewport that displays a buffer:

```cpp
struct Window {
    size_t window_id;
    Buffer* buffer;
    int y, x;           // Position in terminal
    int height, width;  // Dimensions
    size_t scroll_offset;  // First visible line
    int cursor_y, cursor_x;
};
```

### Window-Buffer Relationship

- **Many:Many** — Multiple windows can show the same buffer
- Windows can switch buffers freely via `[call] switch_buffer: buffer_id\n`
- Buffer is destroyed when no windows reference it

## Config System

### Base Implementation

A key-value store read from `config.txt`:

```
# config.txt
editor.tab_width = 4
editor.colorscheme = default
syntax.highlight = enabled
```

### Parsing

Config is parsed once at startup into typed stores:

```cpp
class Config {
    std::unordered_map<std::string, std::string> raw_values;
    
    // Parse config.txt into raw_values
    void load(const std::string& path);
    
    // Typed getters
    int get_int(const std::string& key, int default_value);
    bool get_bool(const std::string& key, bool default_value);
    std::string get_string(const std::string& key, const std::string& default_value);
};
```

### Module Extension

Modules can provide their own config backends by implementing the config interface.

## File Structure

```
hsedit/
├── src/
│   ├── main.cpp              # Entry point
│   ├── editor.cpp            # Editor core (input, render, router)
│   ├── editor.hpp
│   ├── buffer.cpp            # Buffer and Page management
│   ├── buffer.hpp
│   ├── window.cpp            # Window management
│   ├── window.hpp
│   ├── config.cpp            # Config system
│   ├── config.hpp
│   ├── module.cpp            # Module registry and discovery
│   ├── module.hpp
│   └── protocol.hpp          # Protocol constants and helpers
├── modules/
│   ├── syntax-highlight      # Syntax highlighting module
│   ├── file-manager          # File open/save module
│   ├── command-mode          # Command mode module
│   └── ...
├── pages/                    # Page cache directory (created at runtime)
├── config.txt                # User configuration
├── CMakeLists.txt            # Build system
└── TECHNICAL_DESIGN.md       # This file
```

## Data Structures Summary

### Core Types

```cpp
// Protocol messages
struct ProtocolMessage {
    std::string command;
    std::vector<std::string> params;
};

// Module registration
struct ModuleInfo {
    std::string name;
    std::string version;
    std::vector<std::string> handles;      // Commands this module handles
    std::vector<KeyBinding> keybindings;   // Key combinations
    std::vector<std::string> capabilities; // Advanced capabilities
};

// Key binding
struct KeyBinding {
    std::string key;  // e.g., "g g", "Ctrl+C", ":"
    std::string command;
};

// Page
struct Page {
    std::vector<std::string> lines;
    size_t page_id;
    size_t start_line;
    size_t end_line;
    bool modified;
};

// Buffer
struct Buffer {
    size_t buffer_id;
    std::string file_path;
    PageTable page_table;
    bool modified;
    std::unordered_set<size_t> window_refs;
};

// Window
struct Window {
    size_t window_id;
    Buffer* buffer;
    int y, x;
    int height, width;
    size_t scroll_offset;
    int cursor_y, cursor_x;
};
```

## Performance Considerations

1. **Page-based loading**: Only load visible pages into memory
2. **Module on-demand**: Modules spawn only when needed, not kept open
3. **Config parsing**: Parse once at startup, serve typed access
4. **Page splitting**: Automatic splitting prevents memory bloat
5. **Buffer sharing**: Multiple windows can share one buffer (no copy)

## Security Considerations

1. **Module sandboxing**: Modules run as separate processes (fault isolation)
2. **Command validation**: Validate all module responses before executing callbacks
3. **Path validation**: Sanitize file paths in module commands
4. **Resource limits**: Limit module execution time and memory usage
