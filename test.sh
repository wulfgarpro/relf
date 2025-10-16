#!/usr/bin/env bash
./build.sh
./build-debug/relf "${1:-./build-debug/relf}"
