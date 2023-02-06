cmake_minimum_required(VERSION 3.20)
project(YadroTask)

set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.1
)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(src)
add_subdirectory(test)
