cmake_minimum_required(VERSION 2.8)
project(magicline)

set(PROJECT_NAME magicline)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(${PROJECT_NAME}_tmux_SOURCE_FILES
	src/tmux_prompt.cpp)
set(${PROJECT_NAME}_zsh_SOURCE_FILES
	src/zsh_prompt.cpp)

add_executable(${PROJECT_NAME}-tmux ${${PROJECT_NAME}_tmux_SOURCE_FILES})
add_executable(${PROJECT_NAME}-zsh ${${PROJECT_NAME}_zsh_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}-tmux rt)

set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
set(DOCDIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}")
set(EXAMPLE_CONF_DIR ${DOCDIR})

install(TARGETS ${PROJECT_NAME}-tmux ${PROJECT_NAME}-zsh
	RUNTIME DESTINATION ${BINDIR}
	BUNDLE DESTINATION .
)

install(FILES "examples/zshrc" DESTINATION "${EXAMPLE_CONF_DIR}")
install(FILES "examples/tmux.conf" DESTINATION "${EXAMPLE_CONF_DIR}")
install(FILES "examples/tmux-v1.conf" DESTINATION "${EXAMPLE_CONF_DIR}")
