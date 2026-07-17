# JOURNAL.md — hsedit + makebuilder Integration

## Context
hsedit is a TUI text editor project with CMake build system. The user wants to replace manual CMake management with makebuilder, a meta-build system that auto-generates CMakeLists.txt from directory structure + config files.

## Decisions

### 2025-07-17: makebuilder chosen as meta-build system
- makebuilder scans directory tree, reads `.module.config` files, generates CMakeLists.txt
- Each directory = one CMake target
- Supports: StaticLibrary, SharedLibrary, Executable, HeaderOnly, ExternalLibrary
- Multi-config builds (Debug, Dev, Release)
- Handles linker group dependencies for circular lib resolution

### 2025-07-17: src/main.cpp moved to app/main.cpp
- Rationale: makebuilder assigns one target per directory. `src/` contained both library code (module_registry, event_bus) and the main() entry point. Splitting is the natural fit.
- `src/` → StaticLibrary (shared code)
- `app/` → Executable (main entry point)

### 2025-07-17: ncurses configured globally in .project.config
- User requested ncurses be set in .project.config rather than per-module
- Since makebuilder's .project.config doesn't have a direct "linkLibraries" field, ncurses will be linked via customCMake.txt in modules that need it (terminal, tests), OR via a global library reference if supported
- Need to verify this during execution

### 2025-07-17: makebuilder added as git submodule
- `external/makebuilder` is now a git submodule (cloned from git@github.com:mooming/makebuilder.git)
- Submodule keeps the tool versioned alongside the project

### 2025-07-17: Build system fully integrated and verified
- All three targets build in Release, Dev, and Debug configs
- `hsedit` executable, `terminal_module` executable, `hsedit_tests` executable all compile and run
- ncurses found via `find_library` in root `customCMake.txt`, linked via imported target
- `lib/` added to `.gitignore` (build output)
- README.md updated with full build workflow
- Commit: `6f004ba`
