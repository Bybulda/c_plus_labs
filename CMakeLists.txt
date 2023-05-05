cmake_minimum_required(VERSION 3.10)

project(MAI_LABS VERSION 1.0)
add_executable(good_bad_ugly the_good_the_bad_the_ugly/main.cpp ./logger/logger_builder.cpp ./logger/logger_builder_concrete.cpp
        ./logger/logger.cpp ./logger/logger_concrete.cpp memory/memory.cpp the_good_the_bad_the_ugly/alloc_types.cpp)

add_executable(memory memory/main.cpp ./logger/logger_builder.cpp ./logger/logger_builder_concrete.cpp
        ./logger/logger.cpp ./logger/logger_concrete.cpp memory/memory.cpp memory/memory_concrete.cpp)


add_executable(descriptor descriptor/main.cpp ./logger/logger_builder.cpp ./logger/logger_builder_concrete.cpp
        ./logger/logger.cpp ./logger/logger_concrete.cpp memory/memory.cpp descriptor/alloc_types.cpp)

add_executable(logger logger/main.cpp ./logger/logger_builder.cpp ./logger/logger_builder_concrete.cpp
        ./logger/logger.cpp ./logger/logger_concrete.cpp)

add_executable(test_mem try.cpp ./logger/logger_builder.cpp ./logger/logger_builder_concrete.cpp
        ./logger/logger.cpp ./logger/logger_concrete.cpp descriptor/alloc_types.cpp
        memory/memory.cpp memory/memory_concrete.cpp the_good_the_bad_the_ugly/alloc_types.cpp)

add_executable(b_tree B-tree/main.cpp B-tree/b_tree.h ./logger/logger_builder.cpp ./logger/logger_builder_concrete.cpp
        ./logger/logger.cpp ./logger/logger_concrete.cpp memory/memory.cpp memory/memory_concrete.cpp
)