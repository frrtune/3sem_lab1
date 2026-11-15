add_executable(lab1_tests
    tests.cpp
)

target_link_libraries(lab1_tests PRIVATE
    binary_tree
    types
    GTest::gtest_main
)

add_test(NAME lab1_tests COMMAND lab1_tests)