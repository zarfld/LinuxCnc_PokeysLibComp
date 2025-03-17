cmake_minimum_required(VERSION 3.10)

# Project Name
project(pokeys_uspace C)

# Ensure make is available
find_program(MAKE_EXECUTABLE make REQUIRED)

# Set Paths
set(POKEYSLIB_DIR ${CMAKE_SOURCE_DIR}/external/pokeyslib)
set(POKEYSLIB_MAKEFILE ${POKEYSLIB_DIR}/Makefile.noqmake)
set(SUBMAKEFILE_DIR ${CMAKE_SOURCE_DIR}/pokeys_uspace)
set(SUBMAKEFILE ${SUBMAKEFILE_DIR}/Submakefile)

# Clone PoKeysLib if it doesn't exist
if(NOT EXISTS ${POKEYSLIB_DIR})
    message(STATUS "Cloning PoKeysLib into ${POKEYSLIB_DIR}")
    execute_process(
        COMMAND git clone https://bitbucket.org/mbosnak/pokeyslib.git ${POKEYSLIB_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

# Ensure Makefiles exist
if(NOT EXISTS ${POKEYSLIB_MAKEFILE})
    message(FATAL_ERROR "Makefile.noqmake not found in ${POKEYSLIB_DIR}")
endif()
if(NOT EXISTS ${SUBMAKEFILE})
    message(FATAL_ERROR "Submakefile not found in ${SUBMAKEFILE_DIR}")
endif()

# Build pokeyslib using its Makefile
add_custom_target(pokeyslib ALL
    COMMAND ${MAKE_EXECUTABLE} -f ${POKEYSLIB_MAKEFILE}
    WORKING_DIRECTORY ${POKEYSLIB_DIR}
    COMMENT "Building PoKeysLib using Makefile.noqmake"
)

include_directories(
    /usr/include/linuxcnc  # LinuxCNC Headers
    ${CMAKE_SOURCE_DIR}/external/pokeyslib  # PoKeysLib Headers
)

# Build pokeys_uspace, ensuring it depends on pokeyslib
add_custom_target(pokeys_uspace ALL
    COMMAND ${MAKE_EXECUTABLE} -f ${SUBMAKEFILE}
    WORKING_DIRECTORY ${SUBMAKEFILE_DIR}
    COMMENT "Building pokeys_uspace using Submakefile"
    DEPENDS pokeyslib
)

# Install step (optional)
install(DIRECTORY ${SUBMAKEFILE_DIR}/ DESTINATION /usr/lib/linuxcnc/modules
    FILES_MATCHING PATTERN "*.so"
)

