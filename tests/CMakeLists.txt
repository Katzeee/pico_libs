project(test)

include(GoogleTest)

function(create_test test_name)
  file(
    GLOB_RECURSE 
    ${test_name}_test 
    COFIGURE_DEPENDS 
    ${test_name}_test.cpp
  )
  add_executable(${test_name}_test ${${test_name}_test})
  target_link_libraries(
    ${test_name}_test
    GTest::gtest_main
  )
  gtest_discover_tests(${test_name}_test)
endfunction()

if(BUILD_TESTING)
  create_test(mpl)
  target_link_libraries(mpl_test pico_libs::mpl)
  create_test(ecs)
endif()
