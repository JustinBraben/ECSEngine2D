# Set variables used to build the library
# important to add include line to library in your other CMakeLists.txt
set(SFML_COMPONENTS system window graphics network audio)
set(LIBRARY_NAME assignment2)

# Add all header and source files to the library target
file(GLOB_RECURSE HEADER_FILES "*.hpp")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
add_library(${LIBRARY_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Find SFML and add it to your project
find_package(SFML 2.5 REQUIRED COMPONENTS ${SFML_COMPONENTS})

# Loop through SFML_COMPONENTS and create a new list with "sfml-${SFML_COMPONENT}"
set(SFML_LIBRARIES "")
foreach(SFML_COMPONENT ${SFML_COMPONENTS})
    list(APPEND SFML_LIBRARIES "sfml-${SFML_COMPONENT}")
endforeach()

# Link the SFML libraries to your library target
target_link_libraries(${LIBRARY_NAME} ${SFML_LIBRARIES})

# Add SFML include directories to your library target
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})