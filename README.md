# HS Edit

A modular TUI text editor built with C++ and ncurses. HS Edit follows a microservices architecture where core functionality is handled by the base editor, and features are implemented as independent executables (modules) that communicate via standard I/O.

## Features

- **Modular Architecture** — Features are implemented as standalone executables (modules)
- **Microservices Design** — Each module runs as an independent process (fault isolation)
- **On-Demand Execution** — Modules spawn only when needed (efficient resource usage)
- **Page-Based Text Storage** — Handles large files efficiently with paged memory
- **Multi-Window Support** — Multiple viewports, each showing any buffer
- **Configurable** — Key-value config system with module extension points
- **Cross-Platform** — Works on Linux, macOS, and Windows (MSYS2)

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                   HS Edit Editor                    │
├─────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐ │
│  │  Input      │  │  Render     │  │  Router     │ │
│  │  Handler    │  │  Engine     │  │  (Command   │ │
│  │             │  │             │  │   Dispatcher)│ │
│  └─────────────┘  └─────────────┘  └─────────────┘ │
│                       │                             │
│  ┌────────────────────┴─────────────────────────┐   │
│  │           Module Registry                     │   │
│  └──────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────┘
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

```
Module -> Editor: [call] move_cursor: 0, 0\n
Editor -> Module: OK\n
```

## Installation

### Prerequisites

- C++ compiler (GCC, Clang, or MSVC)
- CMake 3.12+
- ncurses library

### Setup Scripts

One-click installation scripts for ncurses:

```bash
# Linux
chmod +x scripts/install-ncurses-linux.sh
./scripts/install-ncurses-linux.sh

# macOS
chmod +x scripts/install-ncurses-macos.sh
./scripts/install-ncurses-macos.sh

# Windows
scripts/install-ncurses-windows.bat
```

### Build

The build system is managed by [makebuilder](https://github.com/mooming/makebuilder), a CMake meta-build system that auto-generates `CMakeLists.txt` from directory structure and `.module.config` files. It is included as a git submodule at `external/makebuilder`.

```bash
# Initialize submodules (if fresh clone)
git submodule update --init --recursive

# Build makebuilder itself
./external/makebuilder/build.sh -release

# Generate CMakeLists.txt from project structure
./external/makebuilder/run.sh /path/to/hsedit

# Build the project (multi-config: Debug, Dev, Release)
cmake -B build -S . -G "Ninja Multi-Config"
cmake --build build --config Release
```

To re-generate CMakeLists.txt after adding or modifying modules:

```bash
./external/makebuilder/run.sh /path/to/hsedit
cmake --fresh -B build -S .
cmake --build build --config Release
```

## Current Progress

### Completed ✅

- [x] Technical design document (`TECHNICAL_DESIGN.md`)
- [x] HTML technical design with SVG diagrams (`docs/TECHNICAL_DESIGN.html`)
- [x] ncurses tutorial (`docs/ncurses-tutorial.html`)
- [x] Platform installation scripts (Linux, macOS, Windows)
- [x] Git repository initialization
- [x] Project structure and build system (makebuilder integration)

### In Progress 🚧

- [ ] Core editor framework (input handler, render engine, router)
- [ ] Module registry and discovery system
- [ ] Protocol implementation (line-based IPC)
- [ ] Page-based text storage

### Planned 📋

- [ ] Base modules:
  - [ ] Command mode module (handles `:` key)
  - [ ] File manager module (open/save files)
  - [ ] Syntax highlighting module
  - [ ] Navigation module (cursor movement)
- [ ] Config system (key-value store)
- [ ] Window/Buffer management
- [ ] Tests and CI/CD

## Development

### Creating a New Module

1. Create a new directory under `modules/`
2. Add a `.module.config` file:

```
name = my_module
buildType = Executable
```

3. Add your source files (e.g., `my_module.cpp`)
4. Re-run makebuilder to generate/update CMakeLists.txt:

```bash
./external/makebuilder/run.sh /path/to/hsedit
cmake --fresh -B build -S .
cmake --build build --config Release
```

5. Implement the `--reveal` command to register capabilities:

```cpp
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--reveal") {
        std::cout << "[call] register: name=\"my-module\", version=\"1.0\"" << std::endl;
        std::cout << "[call] register_key: key=\"m m\", command=\"MY_COMMAND\"" << std::endl;
        return 0;
    }
    
    // Handle commands
    std::string command;
    while (std::getline(std::cin, command)) {
        if (command == "MY_COMMAND") {
            // Do something
            std::cout << "[call] move_cursor: 0, 0" << std::endl;
            std::cout << "DONE" << std::endl;
        }
    }
    return 0;
}
```

### Protocol Reference

See `TECHNICAL_DESIGN.md` for complete protocol specification.

### Directory Structure

```
hsedit/
├── .project.config       # Global build settings (C++17, compile flags)
├── .module.config        # Root module (buildType = None)
├── customCMake.txt       # Global CMake fragments (ncurses setup)
├── app/                  # Main editor executable
│   ├── .module.config
│   └── main.cpp
├── src/                  # Core editor library (StaticLibrary)
│   ├── .module.config
│   ├── module_registry.*
│   └── event_bus.*
├── modules/              # Module executables
│   ├── .module.config
│   └── terminal/
│       ├── .module.config
│       └── terminal_module.cpp
├── tests/                # Test executable
│   ├── .module.config
│   └── test_*.cpp
├── docs/                 # Documentation
├── scripts/              # Setup and build scripts
├── external/
│   └── makebuilder/      # makebuilder (git submodule)
├── CMakeLists.txt        # Auto-generated by makebuilder
├── TECHNICAL_DESIGN.md
└── README.md
```

## Configuration

Configuration is stored in `config.txt`:

```
editor.tab_width = 4
editor.colorscheme = default
syntax.highlight = enabled
```

## Roadmap

### Phase 1: Core Editor (Current)
- [ ] Project structure and build system
- [ ] Core editor framework (input, render, router)
- [ ] Module registry and discovery
- [ ] Protocol implementation

### Phase 2: Base Modules
- [ ] Command mode module
- [ ] File manager module
- [ ] Navigation module
- [ ] Basic syntax highlighting

### Phase 3: Advanced Features
- [ ] Multi-window support
- [ ] Config system
- [ ] Undo/redo
- [ ] Search and replace

### Phase 4: Polish
- [ ] Performance optimization
- [ ] Documentation
- [ ] Tests and CI/CD
- [ ] Plugin marketplace

## Contributing

Contributions are welcome! Please see `TECHNICAL_DESIGN.md` for the complete technical specification.

### Development Setup

1. Clone the repository with submodules:
```bash
git clone --recurse-submodules https://github.com/yourusername/hsedit.git
cd hsedit
```

2. Install dependencies (see Setup Scripts above)

3. Generate CMakeLists.txt and build:
```bash
./external/makebuilder/run.sh /path/to/hsedit
cmake -B build -S . -G "Ninja Multi-Config"
cmake --build build --config Release
```

4. Run:
```bash
./build/app/Release/hsedit
```

## License

This project is licensed under the Apache License, Version 2.0 — see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- ncurses library for terminal UI
- Modern C++ communities for language features
