cmake_minimum_required(VERSION 3.10.3)
project(RayTracer LANGUAGES CXX C)

# Set C/C++ standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add your sources
add_executable(raytracer
  source/private/main.cpp
)

# Include directories
target_include_directories(raytracer
  PRIVATE
  include
  source/public
)

target_compile_options(raytracer
  PRIVATE
  # -Wall
  # -Wextra
)
