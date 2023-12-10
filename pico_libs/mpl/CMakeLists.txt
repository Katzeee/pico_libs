project(mpl)
add_library(${PROJECT_NAME} INTERFACE)
set(lib_headers include/mpl/type_list.hpp)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PICO_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include/mpl>
)

install(FILES ${lib_headers}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mpl
)