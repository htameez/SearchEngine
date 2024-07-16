cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

# testing with the single_include versions of Catch2 V2 (in directory catch2)
include(CTest)

# custom AVL-tree catch2 tests
add_executable(tests_tree test_AVLTree.cpp)
add_test(NAME TestTree COMMAND tests_tree)

# index handler catch2 tests
add_executable(tests_indexhandler test_IndexHandler.cpp IndexHandler.cpp)
add_test(NAME TestIndexHandler COMMAND tests_indexhandler)

# query processor catch2 tests
add_executable(tests_queryProcessor test_QueryProcessor.cpp QueryProcessor.cpp DocumentParser.cpp IndexHandler.cpp SearchEngine.cpp StringCleaner.cpp stemmer/porter2_stemmer.cpp)
add_test(NAME TestQueryProcessor COMMAND tests_queryProcessor)

add_executable(rapidJSONExample rapidJSONExample.cpp StringCleaner.cpp stemmer/porter2_stemmer.cpp DocumentParser.cpp IndexHandler.cpp QueryProcessor.cpp SearchEngine.cpp)
add_executable(supersearch main.cpp StringCleaner.cpp stemmer/porter2_stemmer.cpp DocumentParser.cpp IndexHandler.cpp QueryProcessor.cpp SearchEngine.cpp)

# this makes sure we also link rapidjson
target_include_directories(rapidJSONExample PRIVATE rapidjson/)

set(CMAKE_VERBOSE_MAKEFILE OFF)
