cmake_minimum_required(VERSION 3.16.0)

set(PROJECT_NAME hw_1_14)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -ggdb3")

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

option(BUILD_TESTS "Build all tests." OFF)
if(BUILD_TESTS)

    add_subdirectory(googletest)
    include_directories(googletest/include)

    set(TEST_SOURCES tests)

    file(GLOB test_sources
    ${TEST_SOURCES}/compilation.cpp
    ${TEST_SOURCES}/dataloader.cpp
    ${TEST_SOURCES}/blog.cpp
    ${TEST_SOURCES}/processing.cpp
)

    enable_testing()

    foreach(file ${test_sources})
        set(name)
        get_filename_component(name ${file} NAME_WE)
        add_executable("${name}_tests" ${file})
        target_link_libraries("${name}_tests" PRIVATE gtest)
        target_link_libraries("${name}_tests" PRIVATE ${LIB_NAME})
        add_test(NAME ${name} COMMAND "${name}_tests")
    endforeach()
    
endif()
