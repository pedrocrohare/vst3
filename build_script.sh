#!/bin/bash

# Remove the build directory if it exists
rm -rf build

# Create a new build directory
mkdir build

# Change to the build directory
cd build

# Generate the Xcode project using CMake
cmake -G Xcode ..

# Build the project
cmake --build . 