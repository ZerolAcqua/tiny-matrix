cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(mini-matrix VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")


# configure a header file to pass some of the CMake settings
# to the source code
configure_file(Config.h.in Config.h)

add_subdirectory(Matrix)

# add the executable
add_executable(test test.cpp)

# Use target_link_libraries to link the library to our executable
target_link_libraries(test PUBLIC Matrix)

# Add Matrix to test's target_include_directories()
# Hint: ${PROJECT_SOURCE_DIR} is a path to the project source. AKA This folder!
target_include_directories(test PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Matrix"
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(test PUBLIC
    "${PROJECT_BINARY_DIR}"
)
