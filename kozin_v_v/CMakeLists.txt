cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 17)
project(oop2023kozin)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(doctest REQUIRED)

add_subdirectory(prj.lab)
add_subdirectory(prj.test)
#add_subdirectory(prj.cw)

target_link_libraries(rational matrixs doctest::doctest)