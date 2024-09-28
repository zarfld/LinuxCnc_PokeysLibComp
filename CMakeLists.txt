cmake_minimum_required(VERSION 3.10)

# Set the project name
project(PoKeysLib)

# Add the include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the source files
set(SOURCES
    pokeys_rt/PoKeysLibCore.c
    pokeys_rt/PoKeysLibCoreSockets.c
    pokeys_rt/PoKeysLibDeviceData.c
    pokeys_rt/PoKeysLibIO.c
    pokeys_rt/PoKeysLibEncoders.c
    pokeys_rt/PoKeysLibMatrixLED.c
    pokeys_rt/PoKeysLibMatrixKB.c
    pokeys_rt/PoKeysLibPoNET.c
    pokeys_rt/PoKeysLibLCD.c
    pokeys_rt/PoKeysLibRTC.c
    pokeys_rt/PoKeysLibEasySensors.c
    pokeys_rt/PoKeysLibI2C.c
    pokeys_rt/PoKeysLib1Wire.c
    pokeys_rt/PoKeysLibSPI.c
    pokeys_rt/PoKeysLibPulseEngine_v2.c
    pokeys_rt/PoKeysLibUART.c
    pokeys_rt/PoKeysLibCAN.c
    pokeys_rt/PoKeysLibFailsafe.c
    pokeys_rt/PoKeysLibWS2812.c
    pokeys_rt/PoKeysLibDevicePoKeys57Industrial.c
)

# Add the library
add_library(PoKeysLib SHARED ${SOURCES})

# Set the library properties
set_target_properties(PoKeysLib PROPERTIES
    VERSION 1.0
    SOVERSION 1
)

# Add the executable
add_executable(pokeys_rt pokeys_rt/pokeys_rt.c)

# Link the library to the executable
target_link_libraries(pokeys_rt PoKeysLib)

# Install the library and executable
install(TARGETS PoKeysLib pokeys_rt
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
