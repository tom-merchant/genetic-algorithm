cmake_minimum_required(VERSION 3.21)
project(week1cpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(week1cpp main.cpp individual.h population.h rng.h rng.cpp evaluator.h combinator.h)

Include(FetchContent)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL
        GIT_TAG         release-2.0.22
        GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(SDL2)

set(SDL_LIBRARIES ${SDL_LIBRARIES} mingw32 SDL2main SDL2-static)

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

add_executable(count_ones tests/count_ones.cpp individual.h population.h rng.h rng.cpp evaluator.h combinator.h graphing/GraphModal.cpp graphing/GraphModal.h graphing/SDLPolling.cpp )
target_link_libraries(count_ones ${SDL_LIBRARIES})
target_compile_options(count_ones PRIVATE -march=native -O3 )
target_link_options(count_ones PRIVATE -Dmain=SDL_main)

catch_discover_tests(test_rng)