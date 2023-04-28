# Create a library target from the source files in this directory
add_library(assignment1 main.cpp)

# Include the directory containing this CMake file.
target_include_directories(assignment1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})