add_library(rational STATIC rational.cpp include/rational/rational.hpp)

target_include_directories(rational PUBLIC include)
