#!/bin/bash

# Simple script to build and run tests
echo "Building and running igenius tests..."

# Build tests
make config=debug_x64 igenius_tests

if [ $? -eq 0 ]; then
    echo "Running unit tests..."
    ./bin/Debug/igenius_tests
else
    echo "Build failed!"
    exit 1
fi
