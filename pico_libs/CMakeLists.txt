set(PICO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectory(mpl)
add_library(pico_libs::mpl ALIAS mpl)
add_subdirectory(ecs)
add_library(pico_libs::ecs ALIAS ecs)