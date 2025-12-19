# relf

> A simple ELF reader utility.

`relf` is a simple tool for reading and inspecting ELF (Executable and Linkable Format) binaries. I
created this project primarily as a learning exercise to deepen my understanding on the ELF format.

## Building `relf`

```sh
cmake -S . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --parallel
```

## Running `relf`

```sh
❯ ./relf --help
usage: ./build/relf [OPTION]... <input>
  -h, --file-header
  -l, --program-headers, --segments
  -S, --section-headers, --sections
  -H, --help
```
