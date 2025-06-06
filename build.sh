#!/bin/bash

# Simple build script for FakPlugins
# Usage: ./build.sh [Debug|Release]

# Default to Release if no argument provided
BUILD_TYPE=${1:-Release}

# Validate build type
if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
	echo "Error: Build type must be Debug or Release"
	echo "Usage: $0 [Debug|Release]"
	exit 1
fi

echo "Building with $BUILD_TYPE configuration..."

# Install dependencies with Conan
conan install . --output-folder=build --build=missing -s build_type=$BUILD_TYPE

# Change to build directory
cd build

# Configure with CMake
cmake .. \
	-DCMAKE_TOOLCHAIN_FILE=build/$BUILD_TYPE/generators/conan_toolchain.cmake \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Build
make

echo "Build completed!"
