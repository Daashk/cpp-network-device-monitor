cmake_minimum_required(VERSION 3.16)

project(cpp_network_device_monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    device_monitor
    monitor/main.cpp
)

add_executable(
    device_simulator
    simulator/main.cpp
)