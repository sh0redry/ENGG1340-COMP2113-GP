cmake_minimum_required(VERSION 3.10)
project(Crazy_Thursday VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/Core/*.cpp"
    "src/Utils/*.cpp"
    "src/Counters/*.cpp"
    "src/UI/*.cpp"
    "src/Combat/*.cpp"
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Add executable
add_executable(Crazy_Thursday ${SOURCES})

# Include directories
target_include_directories(Crazy_Thursday PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Core
    ${PROJECT_SOURCE_DIR}/src/Utils
    ${PROJECT_SOURCE_DIR}/src/Counters
    ${PROJECT_SOURCE_DIR}/src/UI
    ${PROJECT_SOURCE_DIR}/src/Combat
)

# Copy resource directories to build directory
file(COPY ${PROJECT_SOURCE_DIR}/ui DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${PROJECT_SOURCE_DIR}/anim DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
