#!/bin/bash
echo "====== Performs cppcheck static code ananysis ======"
for source_file_dir in "project/main.c" "project/include/*.h" "project/source/*.c" "tests/*.cpp"
do
 echo "\n"
 cppcheck $source_file_dir
done
echo "\n================== Check finished ==================="
