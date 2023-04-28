# CMakeList.txt : CMake project for ECSEngine2D, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.20)

message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

project(ecs-engine-cpp CXX)
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(SFML_COMPONENTS system window graphics network audio)

# Add source to this project's executable.
add_executable(myproject ${PROJECT_SOURCE_DIR}/src/main.cpp)

find_package(SFML COMPONENTS ${SFML_COMPONENTS} REQUIRED)

foreach(component IN LISTS SFML_COMPONENTS)
  message(STATUS "Checking SFML component: ${component}")
endforeach()

target_link_libraries(myproject sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET ECSEngine2D PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.
