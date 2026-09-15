# C++ Network Device Telemetry Monitor

A multithreaded C++17 application that simulates network devices
sending telemetry data over UDP and monitors their connectivity state.

The project was built to explore systems programming concepts such as
UDP networking, multithreading, synchronization, RAII, device state
tracking, and automated testing on Linux.

## Features

- Simulates multiple network devices
- Sends telemetry data over UDP/IP
- Parses device telemetry messages
- Maintains the latest state for each device
- Detects devices that stop sending telemetry
- Automatically marks recovered devices as online
- Uses a dedicated receiver thread
- Uses a thread-safe queue for communication between threads
- RAII-based UDP socket management
- Unit tests for telemetry parsing and timeout logic
- CMake-based build system

## Technologies

- C++17
- Linux
- UDP/IP sockets
- STL
- std::thread
- std::mutex
- std::condition_variable
- std::chrono
- CMake
- CTest



## Architecture

```text
Device Simulator
   |
   | UDP telemetry
   v
UDP Receiver Thread
   |
   v
Thread-Safe Queue
   |
   v
Main Processing Thread
   |
   +--> Parse Telemetry
   |
   +--> Update Device State
   |
   +--> Detect ONLINE / OFFLINE Devices