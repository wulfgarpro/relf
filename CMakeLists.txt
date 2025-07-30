cmake_minimum_required(VERSION 3.10)

project(relf C)

add_executable(
  relf
  src/relf.c
  src/main.c
)

target_include_directories(relf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
