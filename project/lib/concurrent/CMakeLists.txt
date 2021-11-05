set(SOURCES source/Matrix.c)

add_library(concurrent SHARED ${SOURCES})

target_include_directories(concurrent
        PUBLIC include/
        )

target_link_libraries(concurrent PUBLIC matrix_interface)