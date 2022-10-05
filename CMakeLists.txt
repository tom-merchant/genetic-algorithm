cmake_minimum_required(VERSION 3.21)
project(week1cpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(week1cpp main.cpp individual.h population.h rng.h rng.cpp evaluator.h combinator.h)
add_executable(count_ones tests/count_ones.cpp individual.h population.h rng.h rng.cpp evaluator.h combinator.h)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)

add_executable(test_rng tests/test_rng.cpp rng.h rng.cpp )
target_link_libraries(test_rng PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(test_rng)