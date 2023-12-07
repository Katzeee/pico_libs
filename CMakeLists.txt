cmake_minimum_required(VERSION 3.16)
project(pico_libs)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Enable testing" ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(mpl)
add_library(pico_libs::mpl ALIAS mpl)
add_subdirectory(ecs)
add_library(pico_libs::ecs ALIAS ecs)

add_subdirectory(tests)