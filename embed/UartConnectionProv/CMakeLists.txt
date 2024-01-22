# SPDX-License-Identifier: Apache-2.0

# Set the Zephyr minimum version required
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

set(DTS_OVERLAY_FILE ${CMAKE_SOURCE_DIR}/board/nrf52833dk_nrf52833.overlay)

# Include the Zephyr build system
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(embed)

# Add the path to the directory containing the missing header file
include_directories(
    include/
)

# Add your source files
FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

zephyr_include_directories(
    ${ZEPHYR_BASE}/subsys/gpio
)

target_link_libraries(app PRIVATE
    zephyr
    )
