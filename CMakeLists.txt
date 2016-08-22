cmake_minimum_required(VERSION 2.8)
project(magicline)

set(PROJECT_NAME magicline)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

execute_process(COMMAND git describe --abbrev=0 --tags OUTPUT_VARIABLE PROJECT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE PROJECT_VERSION_COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(src/project_defines.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/project_defines.h)

set(${PROJECT_NAME}_tmux_SOURCE_FILES
	src/tmux_prompt.c)
set(${PROJECT_NAME}_zsh_SOURCE_FILES
	src/zsh_prompt.cpp)

add_executable(${PROJECT_NAME}-tmux ${${PROJECT_NAME}_tmux_SOURCE_FILES})
add_executable(${PROJECT_NAME}-zsh ${${PROJECT_NAME}_zsh_SOURCE_FILES})

if(NOT CMAKE_SYSTEM_NAME STREQUAL OpenBSD)
	target_link_libraries(${PROJECT_NAME}-tmux rt)
endif()

set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SHAREDIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")
set(EXAMPLE_CONF_DIR ${SHAREDIR})

install(TARGETS ${PROJECT_NAME}-tmux ${PROJECT_NAME}-zsh
	RUNTIME DESTINATION ${BINDIR}
	BUNDLE DESTINATION .
)

install(FILES "share/zshrc" DESTINATION "${EXAMPLE_CONF_DIR}")
install(FILES "share/tmux.conf" DESTINATION "${EXAMPLE_CONF_DIR}")
install(FILES "share/tmux-v1.conf" DESTINATION "${EXAMPLE_CONF_DIR}")
