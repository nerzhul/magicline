cmake_minimum_required(VERSION 3.2)
project(unix_prompt_utils)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
	src/tmux_prompt.cpp)

add_executable(tmux_prompt ${SOURCE_FILES})
