cmake_minimum_required(VERSION 3.13)

project(GSL VERSION 1.0
	DESCRIPTION "GSL project"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB_RECURSE GSL_HEADERS FOLLOW_SYMLINKS *.hpp)
FILE(GLOB_RECURSE GSL_CPPS FOLLOW_SYMLINKS *.cpp)

set(CMAKE_CURRENT_SOURCE_DIR "./src")
set(CMAKE_INCLUDE_PATH "./include" "./include/disruptor")
set(LibSources ${GSL_HEADERS} ${GSL_CPPS})

add_library(GSL OBJECT  ${LibSources})
 
add_library(GSL_static STATIC $<TARGET_OBJECTS:GSL>)

set_target_properties(GSL_static PROPERTIES OUTPUT_NAME GSL) 
 
target_link_libraries(GSL_static PRIVATE GSL)

message("GSL include paths: ${CMAKE_INCLUDE_PATH}")
set(CLIENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "GSL Directory")
message("GSL Dir: ${CLIENT_DIR}")
target_include_directories(GSL PRIVATE 
    ${CMAKE_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS GSL_static ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
