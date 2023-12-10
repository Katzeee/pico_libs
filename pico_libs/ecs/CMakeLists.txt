project(ecs)
add_library(${PROJECT_NAME} INTERFACE)
set(
  lib_headers 
  include/mpl/type_list.hpp
)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PICO_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
add_dependencies(${PROJECT_NAME} mpl)
target_link_libraries(${PROJECT_NAME} INTERFACE mpl)