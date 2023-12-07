project(test)

include(GoogleTest)
function(add_test name)
  file(
    GLOB_RECURSE 
    ${name}_test 
    COFIGURE_DEPENDS 
    ${name}_test.cpp
  )
  add_executable(${name}_test ${${name}_test})
  target_link_libraries(
    ${name}_test
    GTest::gtest_main
  )
  gtest_discover_tests(${name}_test)
endfunction()

if(ENABLE_TEST)
  add_test(mpl)    
  target_link_libraries(mpl_test pico_libs::mpl)
  add_test(ecs)    
endif()
