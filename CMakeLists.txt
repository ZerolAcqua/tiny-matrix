cmake_minimum_required(VERSION 3.10)
project(mini-matrix VERSION 1.0)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


configure_file(Config.h.in Config.h)
add_subdirectory(Matrix)


add_executable(catch_test tests-main.cpp tests-matrix.cpp )


target_link_libraries(catch_test PUBLIC Matrix )
target_include_directories(catch_test PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Matrix"
    # catch.hpp
)
enable_testing()
add_test(
    NAME test
    COMMAND catch_test --success
)

