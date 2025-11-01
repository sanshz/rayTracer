cmake_minimum_required(VERSION 3.10.3)
project(RayTracer LANGUAGES CXX C)

# Set C/C++ standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add your sources
add_executable(main
  Source/Private/main.cpp
)

# Include directories
target_include_directories(main
  PRIVATE
  Include
  Source/Public
)

target_compile_options(main
  PRIVATE
  -Wall
  -Wextra
)
