cmake_minimum_required(VERSION 3.10)

# Project Name
project(pokeys_uspace C)

# Ensure we have make available
find_program(MAKE_EXECUTABLE make REQUIRED)

# Define paths
set(SUBMAKEFILE_DIR ${CMAKE_SOURCE_DIR}/pokeys_uspace)
set(SUBMAKEFILE ${SUBMAKEFILE_DIR}/Submakefile)

# Ensure the Submakefile exists
if(NOT EXISTS ${SUBMAKEFILE})
    message(FATAL_ERROR "Submakefile not found in ${SUBMAKEFILE_DIR}")
endif()

# Add a custom target that runs make using the Submakefile
add_custom_target(pokeys_uspace ALL
    COMMAND ${MAKE_EXECUTABLE} -f ${SUBMAKEFILE}
    WORKING_DIRECTORY ${SUBMAKEFILE_DIR}
    COMMENT "Building pokeys_uspace using Submakefile"
)

# Install step (if required)
install(DIRECTORY ${SUBMAKEFILE_DIR}/ DESTINATION /usr/lib/linuxcnc/modules
    FILES_MATCHING PATTERN "*.so"
)