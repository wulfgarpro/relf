#!/usr/bin/env bash
./build.sh
./build/relf "${1:-./build/relf}"
