# SPDX-License-Identifier: Apache-2.0

# Set the Zephyr minimum version required
cmake_minimum_required(VERSION 3.20.0)

# Include the Zephyr build system
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(embed)

# Add your source files
target_sources(app PRIVATE
    src/main.cpp
    # Add more source files as needed
)

# Specify additional include directories if necessary
target_include_directories(app PRIVATE
    include
    # Add more include directories as needed
)
