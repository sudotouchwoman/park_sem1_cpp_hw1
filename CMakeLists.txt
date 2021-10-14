cmake_minimum_required(VERSION 3.16.0)

set(PROJECT_NAME hw_1_14)
project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -ggdb3 -ftest-coverage -fprofile-arcs")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -ggdb3")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

set(TARGET homework)
set(MAIN project/main.c)
set(LIB_HEADERS project/include)
set(LIB_SOURCES project/source)
set(LIB_NAME blog_lib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/${LIB_HEADERS})

add_library(${LIB_NAME} STATIC
    ${LIB_SOURCES}/post.c
    ${LIB_SOURCES}/blog.c
    ${LIB_SOURCES}/reactions.c
    ${LIB_SOURCES}/dataloader.c
    ${LIB_SOURCES}/processing.c
)

add_executable(${TARGET} ${MAIN})
target_link_libraries(${TARGET} ${LIB_NAME})

option(BUILD_TESTS "Build all tests." ON)
if(BUILD_TESTS)

    add_subdirectory(googletest)
    include_directories(googletest/include)

    set(TEST_SOURCES tests)

    file(GLOB test_sources
    ${TEST_SOURCES}/blog.cpp
    ${TEST_SOURCES}/dataloader.cpp
    ${TEST_SOURCES}/post.cpp
    ${TEST_SOURCES}/processing.cpp
    ${TEST_SOURCES}/reactions.cpp
)

    enable_testing()

    foreach(file ${test_sources})
        set(name)
        get_filename_component(name ${file} NAME_WE)
        add_executable("${name}_tests" ${file})
        target_link_libraries("${name}_tests" PRIVATE gtest_main)
        target_link_libraries("${name}_tests" PRIVATE ${LIB_NAME})
        add_test(NAME ${name} COMMAND "${name}_tests")
    endforeach()
    
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    include(CodeCoverage)
    setup_target_for_coverage_gcovr_xml(
          NAME "${PROJECT_NAME}_coverage"
          EXECUTABLE ctest
          BASE_DIRECTORY "project/")
    
endif()
