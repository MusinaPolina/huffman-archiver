cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RELEASE)

set(Name "hw-02_huffman")
set(DataProcessing "src/reader.cpp" "src/writer.cpp")
set(Archiver "src/archiver.cpp" "src/tree.cpp" "src/node.cpp" "src/code.cpp")
set(Include "include")

project(${Name})

add_executable(${Name} ${DataProcessing} ${Archiver} "src/main.cpp")
target_include_directories(${Name} PUBLIC ${Include})


set(TestName "test_hw-02_huffman")
set(Tests "test/test.cpp" "test/test_data_processing.cpp" "test/test_huffman.cpp")

project(${TestName})

add_executable(${TestName} ${DataProcessing} ${Archiver} ${Tests})
target_include_directories(${TestName} PUBLIC "test/doctest.h" ${Include})
