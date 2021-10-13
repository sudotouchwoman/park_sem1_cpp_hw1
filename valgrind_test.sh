#!/bin/bash
echo "====== Performs valgrind memory leak check ======"
cd build/
for test_suite in blog dataloader post processing reactions
do
 echo "\n"
 valgrind --leak-check=full --track-origins=yes ./$test_suite"_tests"
done
cd ..
echo "\n================ Check finished ================="
