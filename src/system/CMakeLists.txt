cmake_minimum_required (VERSION 3.12)
project (libhsedit_system)

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


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
add_library (libhsedit_system STATIC 
 TextBuffer.cpp
 TextBuffer.h
 VirtualTextBuffer.h
        HSTypes.h
		PageCache.cpp
		PageCache.h
		IndexedItem.h
		StorageIOSystem.cpp
		StorageIOSystem.h
)

if (APPLE)
include_directories ( /System/Library/Frameworks )
endif (APPLE)

install (TARGETS libhsedit_system DESTINATION ${CMAKE_SOURCE_DIR}/lib)



