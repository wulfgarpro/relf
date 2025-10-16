#!/usr/bin/env bash
set -e
./build.sh
./build-debug/relf "${1:-./build-debug/relf}"
