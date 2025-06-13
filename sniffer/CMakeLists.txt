cmake_minimum_required(VERSION 3.4...3.18)
project(sniffer)

set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 REQUIRED)

pybind11_add_module(sniffer sniffer.cpp)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)
include_directories(${PCAP_INCLUDE_DIRS})
link_directories(${PCAP_LIBRARY_DIRS})

target_link_libraries(sniffer PRIVATE ${PCAP_LIBRARIES})
target_link_libraries(sniffer PRIVATE pcap)