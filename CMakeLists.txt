cmake_minimum_required(VERSION 3.1)

project(cs301)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_subdirectory(docs)
else()
	message(STATUS "Doxygen not found, skipping docs")
endif()

add_subdirectory(src)