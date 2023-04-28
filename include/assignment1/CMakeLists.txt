set(SFML_COMPONENTS system window graphics network audio)

find_package(SFML COMPONENTS ${SFML_COMPONENTS})

# Create a library target from the source files in this directory
add_library(assignment1 main.cpp)

target_link_libraries(assignment1 sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# Include the directory containing this CMake file.
target_include_directories(assignment1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})