cmake_minimum_required (VERSION 3.12)
project (hsedit_tests)

set (CMAKE_CXX_STANDARD 23)

if (MSVC)
	add_compile_options (/W3 /WX)
else (MSVC)
	add_compile_options (-Wall -Werror)
endif (MSVC)

include_directories ( ${CMAKE_SOURCE_DIR}
 ${CMAKE_SOURCE_DIR}/include
 )
link_directories (${CMAKE_SOURCE_DIR}/lib)


add_executable (hsedit_tests
 test_main.cpp
 test_module_registry.cpp
 test_protocol.cpp
)

if (APPLE)
include_directories ( /System/Library/Frameworks )
endif (APPLE)

install (TARGETS hsedit_tests DESTINATION ${CMAKE_SOURCE_DIR}/bin)



