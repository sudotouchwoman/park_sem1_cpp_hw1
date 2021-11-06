cmake_minimum_required(VERSION 3.16)
project(HomeWork_2)

# project settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror -Wpedantic)

# absolute path to coverage report tests/unittests/CMakeLists.txt
set(PROJECT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/project)
# set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)


option(CONCURRENT "link concurrent library")
option(NAIVE "link consistent library")
option(STRESS_TEST "Do stress test of libraries")
option(TESTS "Configure unit tests")
option(GTEST_LOCAL "Configured gtest on local machine?")
option(ENABLE_COVERAGE "Build tests coverage statistic")

# check now bc libraries should be built with corresponding flag to enable coverage
if (ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif ()


# build libraries
add_subdirectory("${PROJECT_FOLDER}/lib/")

if (STRESS_TEST OR TESTS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    enable_testing()
    add_subdirectory(tests)
    return()
endif ()


# make executable
add_executable(main ${PROJECT_FOLDER}/main.c)
target_include_directories(main PUBLIC "${PROJECT_FOLDER}/lib/interface/include/")
# link the specified library
if (CONCURRENT)
    target_link_libraries(main concurrent)
elseif (NAIVE)
    target_link_libraries(main naive)
else ()
    message("No library connected to project, default: naive.")
    target_link_libraries(main naive)
endif ()
