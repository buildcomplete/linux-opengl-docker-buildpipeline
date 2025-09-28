#!/bin/bash

# Simple script to build and run tests in release mode
echo "Building and running igenius tests (release)..."

# Build tests
make config=release_x64 igenius_tests

if [ $? -eq 0 ]; then
    echo "Running unit tests (release)..."
    ./bin/Release/igenius_tests
else
    echo "Build failed!"
    exit 1
fi
