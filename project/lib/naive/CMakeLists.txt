set(SOURCES source/Matrix.c source/Processing.c)

add_library(naive STATIC ${SOURCES})

target_include_directories(naive
        PUBLIC include/
        )

target_link_libraries(naive PUBLIC matrix_interface)