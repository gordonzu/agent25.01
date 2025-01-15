#!/bin/bash

clear
mkdir -p build
rm -rf build/src/project_template_run &&
rm -rf build/tests/unit_tests/unit_tests_run
cd build &&
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" &&
make all
rm -rf ../compile_commands.json
ln -s compile_commands.json ../.
./tests/unit_tests/unit_tests_run
#./tests/unit_tests/unit_tests_run --gtest_filter=*TestPercept*-*TestXYEnvironment*
#valgrind --tool=memcheck --leak-check=yes ./tests/unit_tests/unit_tests_run
#valgrind --tool=memcheck --track-origins=yes --leak-check=full --show-leak-kinds=all ./tests/unit_tests/unit_tests_run
