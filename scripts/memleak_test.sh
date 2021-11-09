#!/bin/bash
echo "====== Performs valgrind memory leak check ======"
cd build/
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ctest
cd ..
echo "\n================ Check finished ================="
