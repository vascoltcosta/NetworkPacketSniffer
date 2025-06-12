cmake_minimum_required(VERSION 3.4...3.18)
project(sniffer)

set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 REQUIRED)

pybind11_add_module(sniffer sniffer.cpp)