add_executable(core_lib_tests test_core.cpp)
target_link_libraries(core_lib_tests PRIVATE ratioCore SeMiTONE)

add_test(NAME CORE_LibTest COMMAND core_lib_tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR})