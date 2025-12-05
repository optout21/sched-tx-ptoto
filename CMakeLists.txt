cmake_minimum_required(VERSION 3.10)

project(schedtx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(schedtx bitcoincore.cpp schedtx.cpp main.cpp)
