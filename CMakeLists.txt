cmake_minimum_required (VERSION 3.12)
project (hsedit)

set (CMAKE_CONFIGURATION_TYPES "Debug;Dev;Release" CACHE STRING "" FORCE)
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif ()

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set (CMAKE_CXX_FLAGS_DEV "${CMAKE_CXX_FLAGS_DEBUG} -O1")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set (CMAKE_CXX_STANDARD 17)

if (APPLE)
    set(PLATFORM_SOURCES
    )
else()
    set(PLATFORM_SOURCES "")
endif()

if (MSVC)
	add_compile_options (/W3 /WX)
else (MSVC)
	add_compile_options (-Wall -Werror)
endif (MSVC)

include_directories ( ${CMAKE_SOURCE_DIR}
 ${CMAKE_SOURCE_DIR}/include
 )
add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (modules)




# Global CMake setup - ncurses
find_library(NCURSES_LIBRARY ncurses)
if(NCURSES_LIBRARY)
    add_library(ncurses UNKNOWN IMPORTED)
    set_target_properties(ncurses PROPERTIES
        IMPORTED_LOCATION "${NCURSES_LIBRARY}"
    )
    message(STATUS "ncurses found: ${NCURSES_LIBRARY}")
else()
    message(WARNING "ncurses not found - linking will fail for modules that need it")
endif()

