# CMakeList.txt : CMake project for ECSEngine2D, include source and define
# project specific logic here.
#

# Set the minimum required version of CMake.
cmake_minimum_required(VERSION 3.20)

# Print the CMake toolchain file path.
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

# Ensure that we are not building in the source directory.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Building in the source directory is not allowed. Please use a separate build directory.")
endif()

# Define the project and set the C++ standard to 17.
# ${PROJECT_NAME} will now be myproject
project(myproject CXX)
set(CMAKE_CXX_STANDARD 17)

# Set the binary directory to /build within the source directory.
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Define the SFML components to use.
set(SFML_COMPONENTS system window graphics network audio)

# Add any library files you want
#add_subdirectory(${PROJECT_SOURCE_DIR}/include/assignment1)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/assignment2)

# Add source files to the project's executable and include the src directory.
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Enforce treating warnings as errors for GNU and Clang compilers.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -O3 -g -fPIC -fsanitize=address -fsanitize=undefined --coverage)
endif()

# Enforce treating warnings as errors for MSVC compiler.
if(MSVC)
  # Treat warnings as errors
  target_compile_options(${PROJECT_NAME} PRIVATE /WX)
  
  # Enable debugging information and code coverage.
  target_compile_options(${PROJECT_NAME} PRIVATE /Zi /DEBUG /coverage)
  
  # Enable code analysis
  target_compile_options(${PROJECT_NAME} PRIVATE /analyze)
endif()

# Find and require the SFML library, ensuring it is at least version 2.5.
find_package(SFML COMPONENTS ${SFML_COMPONENTS} REQUIRED)
if(SFML_VERSION VERSION_LESS "2.5")
  message(FATAL_ERROR "SFML version must be at least 2.5")
endif()

# Print the SFML components being used.
foreach(component IN LISTS SFML_COMPONENTS)
  message(STATUS "Checking SFML component: ${component}")
endforeach()

# Link the SFML libraries to the project's executable.
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio assignment2)

# TODO: Add tests and install targets if needed.
