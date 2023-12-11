# SPDX-License-Identifier: Apache-2.0

# Set the Zephyr minimum version required
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Include the Zephyr build system
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
#set(DTS_OVERLAY_FILE ${CMAKE_SOURCE_DIR}/board/nucleo_f091rc.overlay)
set(DTS_OVERLAY_FILE ${CMAKE_SOURCE_DIR}/board/nrf52833dk_nrf52833.overlay)
#set(DTS_OVERLAY_FILE ${CMAKE_SOURCE_DIR}/board/nrf52840dongle_nrf52840.overlay)
project(bluetoothUartMesh)

# Add the path to the directory containing the missing header file
include_directories(
)

# Add your source files
target_sources(app PRIVATE
    src/main.c
    src/bluetooth.c
    src/board.c
    # Add more source files as needed
)

zephyr_include_directories(
    ${ZEPHYR_BASE}/subsys/gpio
)

target_link_libraries(app PRIVATE
    zephyr
    )

    
project(UartConnection)

#FILE(GLOB app_sources src/*.c)
#target_sources(app PRIVATE ${app_sources})
