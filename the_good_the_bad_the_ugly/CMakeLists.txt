cmake_minimum_required(VERSION 3.10)

# Name the project and set some variables
project(MEMORY VERSION 1.0 LANGUAGES CXX)

# Define an executable target called hello from main.cpp
add_executable(main main.cpp ../logger/logger_builder.cpp ../logger/logger_builder_concrete.cpp
        ../logger/logger.cpp ../logger/logger_concrete.cpp memory.cpp alloc_types.cpp)


# cmake -B C:\Users\smirn\github\C_plus_labs\build -S . -DCMAKE_TOOLCHAIN_FILE=C:/c++_libs/vcpkg/scripts/buildsystems/vcpkg.cmake
#find_path(FORMAT format.h C:/c++_libs/vcpkg/packages/fmt_x86-windows/include/fmt)
# Install the hello target into bin directory
# install(TARGETS hello DESTINATION bin)