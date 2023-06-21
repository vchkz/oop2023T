add_executable(test_rational test_rational.cpp)
add_executable(test_arrayd test_arrayd.cpp)
add_executable(test_arrayt test_arrayt.cpp)
add_executable(test_matrixs test_matrixs.cpp)

target_link_libraries(test_rational rational doctest::doctest)
target_link_libraries(test_arrayd arrayd doctest::doctest)
target_link_libraries(test_arrayt arrayt doctest::doctest)
target_link_libraries(test_matrixs matrixs doctest::doctest)
