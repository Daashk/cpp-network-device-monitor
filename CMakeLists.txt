cmake_minimum_required(VERSION 3.16)

project(cpp_network_device_monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(
    device_monitor
    monitor/main.cpp
    src/Telemetry.cpp
)

target_include_directories(
    device_monitor
    PRIVATE include
)

target_link_libraries(
    device_monitor
    PRIVATE Threads::Threads
)

add_executable(
    device_simulator
    simulator/main.cpp
)

