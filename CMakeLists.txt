cmake_minimum_required(VERSION 3.10)
project(hsedit VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ncurses
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(NCURSES QUIET ncursesw)
    if(NOT NCURSES_FOUND)
        pkg_check_modules(NCURSES QUIET ncurses)
    endif()
endif()

if(NOT NCURSES_FOUND)
    find_path(NCURSES_INCLUDE_DIR ncurses.h)
    find_library(NCURSES_LIBRARY NAMES ncurses ncursesw)
    if(NCURSES_INCLUDE_DIR AND NCURSES_LIBRARY)
        set(NCURSES_FOUND TRUE)
        set(NCURSES_INCLUDE_DIRS ${NCURSES_INCLUDE_DIR})
        set(NCURSES_LIBRARIES ${NCURSES_LIBRARY})
    endif()
endif()

if(NOT NCURSES_FOUND)
    message(FATAL_ERROR "ncurses not found. Install with:\n  Linux: sudo apt install libncurses-dev\n  macOS: brew install ncurses\n  Windows: pacman -S mingw-w64-x86_64-ncurses")
endif()

# Core editor (no ncurses dependency here — Terminal Module will link it)
add_executable(hsedit
    src/main.cpp
    src/module_registry.cpp
    src/event_bus.cpp
)

target_include_directories(hsedit PRIVATE src)

# Terminal Module (links ncurses)
add_executable(terminal_module
    modules/terminal/terminal_module.cpp
)

target_include_directories(terminal_module PRIVATE modules/terminal)
target_link_libraries(terminal_module PRIVATE ${NCURSES_LIBRARIES})
target_link_directories(terminal_module PRIVATE ${NCURSES_LIBRARY_DIRS})

# Test executable
add_executable(hsedit_tests
    tests/test_main.cpp
    tests/test_protocol.cpp
    tests/test_module_registry.cpp
    src/module_registry.cpp
    src/event_bus.cpp
)

target_include_directories(hsedit_tests PRIVATE src tests)
target_link_libraries(hsedit_tests PRIVATE ${NCURSES_LIBRARIES})

# Install
install(TARGETS hsedit DESTINATION bin)
install(TARGETS terminal_module DESTINATION bin)
