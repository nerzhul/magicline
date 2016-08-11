cmake_minimum_required(VERSION 3.2)
project(magicline)

set(PROJECT_NAME magicline)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
	src/tmux_prompt.cpp)

add_executable(${PROJECT_NAME}-tmux ${SOURCE_FILES})

set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
set(DOCDIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}")
set(EXAMPLE_CONF_DIR ${DOCDIR})

install(TARGETS ${PROJECT_NAME}-tmux
	RUNTIME DESTINATION ${BINDIR}
	BUNDLE DESTINATION .
)
