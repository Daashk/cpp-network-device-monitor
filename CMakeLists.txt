cmake_minimum_required(VERSION 3.16)

project(cpp_network_device_monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

find_package(Threads REQUIRED)

add_executable(
    device_monitor
    monitor/main.cpp
    src/Telemetry.cpp
    src/UdpSocket.cpp
)

target_include_directories(
    device_monitor
    PRIVATE
    include
)

target_link_libraries(
    device_monitor
    PRIVATE
    Threads::Threads
)

add_executable(
    device_simulator
    simulator/main.cpp
    src/UdpSocket.cpp
)

target_include_directories(
    device_simulator
    PRIVATE
    include
)

enable_testing()

add_executable(
    device_tests
    tests/tests.cpp
    src/Telemetry.cpp
)

target_include_directories(
    device_tests
    PRIVATE
    include
)
 
add_test(
    NAME DeviceTests
    COMMAND device_tests
)