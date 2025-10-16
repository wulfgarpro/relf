#!/usr/bin/env bash
set -e
# Debug
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-debug --parallel

ln -sf "build-debug/compile_commands.json" "$(pwd)/compile_commands.json"

# Release
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel
