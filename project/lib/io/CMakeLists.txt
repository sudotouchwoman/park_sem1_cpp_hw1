set(SOURCES source/MatrixIO.c)

add_library(io STATIC ${SOURCES})

target_include_directories(io PUBLIC include/)

target_link_libraries(io PUBLIC matrix_interface)