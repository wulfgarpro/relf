cmake_minimum_required(VERSION 3.13)

project(relf C)

add_executable(
  relf
  src/cli.c
  src/relf.c
  src/main.c
)

target_include_directories(relf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Compile-time flags for Debug
target_compile_options(relf PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer>
    $<$<CONFIG:Debug>:-Wall -Wextra -Werror>
  )
# Link-time flags for Debug
target_link_options(relf PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined>
  )
