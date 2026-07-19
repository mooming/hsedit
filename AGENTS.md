//
// Created by hsedit project on 2026. 7. 19.
//

# AGENTS.md — HardBop Engine (hsedit)

This document defines the coding standards, architectural conventions, and toolchain rules that every agent must follow in this repository. The **primary reference** is `src/standards/CodingStandards.{h,cpp}` — every code convention below is extracted from that file or derived from observed project code. **When in doubt, mimic `src/standards/CodingStandards.{h,cpp}`.**

---

## 1. Project Overview

| Item | Value |
|---|---|
| **Language** | C++23 |
| **Build system** | [makebuilder](https://github.com/mooming/makebuilder) (CMake meta-build) |
| **Namespace** | `hs` (sub-namespaces: `hs::system`, `hs::ui`) |
| **Warning policy** | `-Wall -Werror` (GCC/Clang), `/W3 /WX` (MSVC) |
| **Editor** | CLion |
| **Target** | Modular TUI text editor |

---

## 2. MakeBuilder (Build System)

makebuilder is a CMake meta-build system that auto-generates `CMakeLists.txt` from directory structure and `.module.config` files. It is included as a git submodule at `external/makebuilder`.

### 2.1 `.module.config` Format

Each directory may contain a `.module.config` file:

```ini
name = libhsedit_system        # Target name (omit for containers)
buildType = StaticLibrary      # Executable | StaticLibrary | None (container) | Ignored
dependency = libhsedit_ui      # Link dependency (repeatable)
library = ncurses              # System library to link (repeatable)
```

**Examples from this project:**

```ini
# src/system/.module.config
name = libhsedit_system
buildType = StaticLibrary

# src/app/.module.config
name = hsedit
buildType = Executable
dependency = libhsedit_module
dependency = libhsedit_ui
dependency = libhsedit_system

# modules/terminal/.module.config
name = terminal_module
buildType = Executable
library = ncurses
```

### 2.2 `.project.config` (Root)

Global project settings at the root:

```ini
requiredCMakeVersion = 3.12
cxxStandard = 23
compileOptions = -Wall -Werror
msvcCompileOptions = /W3 /WX
```

### 2.3 `customCMake.txt` (Root)

Global CMake fragments appended to the generated `CMakeLists.txt`. Used for library discovery (e.g., ncurses):

```cmake
find_library(NCURSES_LIBRARY ncurses)
if(NCURSES_LIBRARY)
    add_library(ncurses UNKNOWN IMPORTED)
    set_target_properties(ncurses PROPERTIES
        IMPORTED_LOCATION "${NCURSES_LIBRARY}"
    )
endif()
```

### 2.4 Creating a New Module

1. Create directory under `modules/`
2. Add `.module.config`:

```ini
name = my_module
buildType = Executable
```

3. Add source files (e.g., `my_module.cpp`)
4. Re-generate:

```bash
./external/makebuilder/run.sh /path/to/hsedit
cmake --fresh -B build -S . -G "Ninja Multi-Config"
cmake --build build --config Release
```

### 2.5 Adding a New System

1. Create `src/system/MyNewSystem.{h,cpp}`
2. Add `.module.config` if the directory needs its own target (otherwise files go into `libhsedit_system`)
3. Re-generate with makebuilder

---

## 3. Directory Structure

```
hsedit/
├── .project.config              # Global build settings
├── .module.config               # Root container (buildType = None)
├── customCMake.txt              # Global CMake fragments
├── external/
│   └── makebuilder/             # makebuilder (git submodule)
├── src/
│   ├── app/main.cpp             # Application entry point
│   ├── module/                  # Plugin module system
│   ├── standards/               # Coding standards reference (DO NOT MODIFY)
│   ├── system/                  # Core systems (buffers, I/O, storage)
│   └── ui/                      # UI layer (ncurses)
├── modules/                     # External module executables
├── tests/                       # Test executables
├── docs/                        # Design documents
└── scripts/                     # Setup scripts
```

- System classes live in `src/system/`, UI classes in `src/ui/`, modules in `src/module/`.
- New system: create `.h` + `.cpp` pair in `src/system/`, register in `System.{h,cpp}`.
- Module executables live in `modules/<name>/`.

---

## 4. Include & File Header Rules

### 4.1 File Header

```cpp
//
// Created by <author> on <date>.
//

#pragma once
```

Every `.h` and `.cpp` starts with a blank line, then the creation comment (`Created by ...`), then a blank line, then `#pragma once`. **Do not use `#ifndef`/`#define` include guards** — `#pragma once` is the project standard.

### 4.2 Include Order

1. **Own header** (first, no blank line before it)
2. **Standard library headers** (alphabetical order)
3. **Project headers** (alphabetical order)

After the last include, place **two blank lines** before the first code body.

```cpp
#include "TextBuffer.h"           // 1. Own header

#include <algorithm>              // 2. Standard library (alphabetical)
#include <iostream>
#include <string>

#include "HSTypes.h"              // 3. Project headers
```

---

## 5. Naming Conventions

| Category | Rule | Example |
|---|---|---|
| **Namespaces** | lowercase, dotted | `hs`, `hs::system`, `hs::ui` |
| **Classes / structs / enums** | PascalCase | `VirtualTextBuffer`, `ModuleInfo` |
| **Functions / methods** | camelCase | `Open()`, `Initialize()`, `AddLine()` |
| **Member variables** | camelCase with trailing `_` | `initialized_`, `cacheSizeInBytes_` |
| **Local variables** | camelCase (no trailing `_`) | `filePath`, `deltaTime`, `handle` |
| **Type aliases** | `T` prefix | `TLine`, `TIndex`, `TFilePath` |
| **Constants (`static constexpr`)** | PascalCase with descriptive prefix | `MaxValue`, `DefaultVersion` |
| **Template parameters** | `T` prefix | `TEntry`, `TValue` |
| **Interfaces** | `I` prefix | `ICodingStandards` |
| **Structs (POD)** | PascalCase, no trailing `_` on members | `CodingStandardsData.version` |

### 5.1 Parameter Prefixes

| Prefix | Meaning | Example |
|---|---|---|
| `in` | Input (avoids collision with member) | `SetValue(int inValue)` |
| `out` | Write-only reference parameter | `TryParse(..., int& outResult)` |
| `inOut` | Read-write reference parameter | `ClampToRange(int& inOutValue, ...)` |

---

## 6. Formatting Rules

- **Indentation**: Tabs (not spaces).
- **Column limit**: 120 characters.
- **Trailing newline**: Every file ends with `\n`.
- **Namespace bodies**: NOT indented (flat indentation under `namespace`).

### 6.1 Brace Style (Allman / BSD)

**All** opening braces go on the **next line**, at the **same indentation level** as the declaration — functions, classes, namespaces, and control flow:

```cpp
void Foo()
{
	if (value > 0)
	{
		value++;
	}
}

class Bar
{
	int x;
};
```

Note: `{` is **not** indented one level deeper than its declaration — it sits at the same column as the keyword or type name above it.

### 6.2 Single-Line Statements

Omit braces for single-line `return`/`continue`:

```cpp
void Foo()
{
    if (!initialized_)
        return;
    // ...
}
```

### 6.3 Readability Whitespace

- Empty line before `return` (unless it is the only statement in scope).
- Empty line after closing `}`.
- Empty line between member variables and methods.

---

## 7. Class Design

### 7.1 Prefer Composition Over Inheritance

```cpp
// GOOD: Composition — dependency is explicit
class Processor final
{
private:
    DataProcessor processor;   // Has-a relationship
    int extraData;
};

// BAD: Inheritance for component reuse
class BadProcessor final : private DataProcessor { ... };
```

### 7.2 `final` on Non-Inheritable Classes

```cpp
class System final { ... };
```

### 7.3 Virtual Methods

- CodingStandards defines `ICodingStandards` as a pure virtual interface (`= 0`) with `I` prefix. **Follow this pattern if a virtual interface is needed.**
- **In the actual project, all classes are `final` and composition is used.** Virtual methods are not used in production code.

### 7.4 Copy/Move Semantics

```cpp
System(const System&) = delete;
System& operator=(const System&) = delete;
System(System&&) noexcept = default;
System& operator=(System&&) noexcept = default;
```

- **No exceptions** in this engine: no `try`/`catch`/`throw`.
- Mark move operations `noexcept`.

### 7.5 Member Ordering

```
1. Public static members / constants
2. Public methods
3. Protected methods
4. Private methods
5. Private member variables
```

### 7.6 Getters

- Prefix with `Get`: `GetBuffer()`, `GetStoragePath()`.
- Mark `[[nodiscard]]` on all getters and value-returning functions.
- Prefer `auto&` return for container accessors:

```cpp
[[nodiscard]] auto& GetBuffer() { return lines; }
[[nodiscard]] auto& GetBuffer() const { return lines; }
```

---

## 8. Special Member Functions

### 8.1 Constructors

- Use **member initializer lists** (not assignment in body):

```cpp
System::System()
    : storageIOSystem_(std::make_unique<StorageIOSystem>())
    , virtualTextBufferSystem_(std::make_unique<VirtualTextBufferSystem>())
    , initialized_(false)
{
}
```

- `explicit` on single-argument constructors (prevents implicit conversion).
- `= default` for trivial constructors/destructors/copy/move.

### 8.2 Destructors

```cpp
~System() = default;        // Trivial
~VirtualTextBufferSystem(); // Non-trivial: call Stop() to clean up threads
```

Non-trivial destructors **must** clean up resources (threads, file handles, etc.).

### 8.3 Rule of Zero

Prefer smart pointers (`unique_ptr`, `shared_ptr`) to enforce RAII. Avoid raw `new`/`delete` in new code.

---

## 9. Threading & Concurrency

- Use `std::mutex` + `std::lock_guard` for critical sections.
- Use `std::condition_variable` with `std::unique_lock` for worker threads.
- Background threads: join in destructor or explicit `Stop()`.

```cpp
std::mutex mutex_;
std::condition_variable condition_;
std::thread workerThread_;

void Update(float deltaTime)
{
    std::lock_guard<std::mutex> lock(mutex_);
    ...
    condition_.notify_one();
}
```

- Never call `join()` without checking `joinable()` first.
- Worker thread loop pattern:

```cpp
void WorkerThread()
{
    while (running_)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait_for(lock, std::chrono::milliseconds(100));
        if (!running_) break;
        CheckFileChanges();
    }
}
```

---

## 10. Memory Management

### 10.1 Stack Allocation Preferred

```cpp
int buffer[256];
for (int i = 0; i < 256; ++i)
{
    buffer[i] = i * i;
}
```

### 10.2 Smart Pointers for Heap

- `std::unique_ptr<T>` for exclusive ownership (system internals).
- `std::shared_ptr<T>` for shared ownership (buffers visible to users).
- **Never use raw `new`/`delete`** in new code.

### 10.3 RVO / NRVO

- Return by value to enable RVO/NRVO. **Do not `std::move` on return** (prevents NRVO):

```cpp
InlinedData Create() noexcept
{
    InlinedData result(42);
    return result;     // ← NRVO: compiler elides the copy/move
}

// BAD:
return std::move(result);   // ← prevents NRVO
```

- DO use `std::move` when passing to a `T&&` sink parameter or in move constructors/assignments.

---

## 11. Error Handling (Exception-Free Engine)

This engine uses **no exceptions**. All error handling follows these rules:

### 11.1 Early Returns

```cpp
void Foo()
{
    if (!initialized_)
    {
        return;
    }
    // ... normal path
}
```

### 11.2 `Assert()` for Invariants

```cpp
Assert(ptr != nullptr);
Assert(inputSize >= 0);
Assert(false && "Invalid state");
```

Assert macro is demonstrated in `src/standards/CodingStandards.h`. Use it for critical invariant checks.

### 11.3 Log Before Assert / Return

When an error or warning condition is detected, **always log before returning or asserting**:

```cpp
if (inputSize > static_cast<int>(MaxNameLength))
{
    std::cerr << "Warning: inputSize (" << inputSize
              << ") exceeds MaxNameLength (" << MaxNameLength
              << "). Clamping to MaxNameLength." << std::endl;
    inputSize = MaxNameLength;
}
```

- Use distinct prefixes: `"Error:"`, `"Warning:"`, `"Validation Error:"`.
- Include relevant state values in log messages.

---

## 12. Compile-Time Validation

- **`static_assert`** for struct sizes, constant ranges, template constraints:

```cpp
static_assert(MaxValue > 0, "MaxValue must be positive");
static_assert(std::is_integral_v<TEntry>, "Requires integral type");
```

- **`constexpr`** for compile-time constant values.
- **`consteval`** (C++20) for functions that must evaluate at compile time.

---

## 13. STL & Language Features

### 13.1 Iteration

- **Prefer range-based for** over index-based loops:

```cpp
for (const auto& v : values)
{
    Assert(v >= 0);
}
```

### 13.2 Key Types

- `std::vector<T>` — ordered collections
- `std::unordered_map<K, V>` — hash maps
- `std::filesystem::path` (alias: `TFilePath`) — file paths
- `std::chrono::steady_clock::time_point` (alias: `TTimestamp`) — timestamps
- `std::u8string` (alias: `TLine`) — UTF-8 text

### 13.3 Move Semantics

```cpp
// In move constructor/assignment:
data = other.data;
length = other.length;
other.data = nullptr;    // ← leave source in valid empty state
other.length = 0;
```

### 13.4 `[[nodiscard]]`

Apply to all getters and functions whose return value is meaningful:

```cpp
[[nodiscard]] const char* GetText() const noexcept;
[[nodiscard]] bool IsOpen(const std::filesystem::path& filePath) const;
```

### 13.5 `noexcept`

Mark functions `noexcept` when they cannot throw. Omit when in doubt — a broken `noexcept` contract calls `std::terminate`. Functions with no allocation, no virtual calls, and no external I/O are safe to mark.

---

## 14. Singleton Pattern

```cpp
static System& GetInstance()
{
    static System instance;   // Meyers singleton — thread-safe in C++11+
    return instance;
}
```

- Private constructors.
- Delete copy; allow move.
- One instance per process.

---

## 15. Module System

Modules are standalone executables that communicate via standard I/O with the editor:

```
Module -> Editor: [call] move_cursor: 0, 0\n
Editor -> Module: OK\n
```

### 15.1 Module Lifecycle

1. **Discover**: Located in `modules/` directory
2. **Register**: Called with `--reveal` flag to announce capabilities
3. **Execute**: Spawned on-demand when their command is invoked
4. **Communicate**: Use standard I/O with line-based protocol

### 15.2 Module `--reveal` Example

```cpp
int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--reveal") {
        std::cout << "[call] register: name=\"my-module\", version=\"1.0\"" << std::endl;
        return 0;
    }
    // Handle commands via stdin
    std::string command;
    while (std::getline(std::cin, command)) {
        if (command == "MY_COMMAND") {
            std::cout << "[call] move_cursor: 0, 0" << std::endl;
            std::cout << "DONE" << std::endl;
        }
    }
    return 0;
}
```

---

## 16. Build & Run

```bash
# Initialize submodules (if fresh clone)
git submodule update --init --recursive

# Build makebuilder itself
./external/makebuilder/build.sh -release

# Generate CMakeLists.txt from project structure
./external/makebuilder/run.sh /path/to/hsedit

# Configure
 cmake -B build -S . -G "Ninja Multi-Config"

# Build (Release)
cmake --build build --config Release

# Run tests
./build/tests/hsedit_tests

# Run application
./build/app/Release/hsedit
```

To re-generate CMakeLists.txt after adding or modifying modules:

```bash
./external/makebuilder/run.sh /path/to/hsedit
cmake --fresh -B build -S .
cmake --build build --config Release
```

---

## 17. Text Buffer

HS Edit uses a paged text buffer with these characteristics:

- **UTF-8 Native**: Uses `std::u8string` for all text storage (`TLine`).
- **Efficient Operations**: Line operations use move semantics and `emplace_back`.
- **Automatic Newline Splitting**: Insertion operations split input by `\n`.
- **Split Capability**: Buffers can be split at any line boundary.

---

## Summary Checklist for Every Code Change

Before committing, verify:

- [ ] File has correct header (`Created by ...` + `#pragma once`)
- [ ] Includes are in correct order (own → standard → project, alphabetical)
- [ ] Naming follows conventions (classes=PascalCase, members=camelCase_, types=T-prefix)
- [ ] Indentation is tabs, not spaces
- [ ] Braces follow Allman/BSD style (next line, same indentation as declaration)
- [ ] Single-line returns/continues have no braces
- [ ] Getters are `[[nodiscard]]`
- [ ] Classes are `final` unless intentionally inheritable
- [ ] Composition over inheritance
- [ ] No raw `new`/`delete` (use smart pointers)
- [ ] No exceptions (no try/catch/throw)
- [ ] Move operations are `noexcept`
- [ ] Member initializer lists used in constructors
- [ ] Thread safety: mutex + lock_guard in place
- [ ] `static_assert` for critical invariants
- [ ] File ends with trailing newline