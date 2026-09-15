# C++ Network Device Telemetry Monitor

![C++ CI](https://github.com/Daashk/cpp-network-device-monitor/actions/workflows/ci.yml/badge.svg)

A multithreaded C++17 application that simulates network devices sending telemetry data over UDP and monitors their connectivity state.

The project demonstrates practical systems programming concepts including UDP networking, multithreading, synchronization, RAII, resource ownership, device state tracking, input validation, and automated testing on Linux.

## Features

- Simulates multiple network devices
- Sends telemetry data over UDP/IP
- Parses and validates telemetry messages
- Maintains the latest state for each device
- Tracks the last time telemetry was received
- Detects devices that stop sending data
- Automatically marks recovered devices as online again
- Uses a dedicated UDP receiver thread
- Uses a thread-safe queue for communication between threads
- Uses RAII for automatic socket resource management
- Includes automated tests for telemetry parsing and timeout logic
- Builds with CMake
- Runs automated build and test checks with GitHub Actions

## Technologies

- C++17
- Linux
- UDP/IP sockets
- STL
- `std::thread`
- `std::mutex`
- `std::condition_variable`
- `std::chrono`
- `std::unordered_map`
- RAII
- Move semantics
- CMake
- CTest
- GitHub Actions

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
       +------> Parse and validate telemetry
       |
       +------> Update device state
       |
       +------> Track last-seen timestamp
       |
       +------> Detect ONLINE / OFFLINE devices
```

The receiver thread is responsible only for receiving network packets.

Received messages are placed into a thread-safe queue. The main processing thread retrieves messages from the queue, parses the telemetry, updates device state, and periodically checks whether devices have exceeded the offline timeout.

This separation prevents telemetry processing from blocking network reception.

## Telemetry Format

Each simulated device sends telemetry using the following format:

```text
DEVICE_ID|TEMPERATURE|SIGNAL_STRENGTH|SEQUENCE_NUMBER
```

Example:

```text
DEVICE01|42.5|-63|12
```

Fields:

- `DEVICE01` — device identifier
- `42.5` — temperature in Celsius
- `-63` — signal strength in dBm
- `12` — sequence number

Malformed telemetry is rejected instead of terminating the monitor.

## Device Monitoring

The monitor stores the latest state for every known device.

For each device it tracks:

- latest telemetry
- last received timestamp
- online/offline state

If no telemetry is received from a device within the configured timeout, the device is marked offline.

When telemetry from that device is received again, it automatically transitions back online.

Example:

```text
DEVICE01 is ONLINE
DEVICE02 is ONLINE
DEVICE03 is ONLINE

*** DEVICE03 is OFFLINE ***

DEVICE03 is ONLINE again
```

## RAII and Resource Ownership

UDP sockets are managed by the `UdpSocket` class.

The socket file descriptor is acquired when the object is constructed and automatically released when the object is destroyed.

Copy operations are disabled to prevent multiple objects from owning the same file descriptor.

Move operations are supported so socket ownership can be transferred safely.

This demonstrates RAII-based resource management and explicit ownership semantics in C++.

## Thread-Safe Queue

Communication between the UDP receiver thread and the main processing thread is handled by a thread-safe queue.

The queue uses:

- `std::mutex` to protect shared data
- `std::condition_variable` to wait efficiently for incoming messages
- `std::unique_lock` and `std::lock_guard` for synchronization

The main thread uses a timed wait so it can continue checking device timeout conditions even when no telemetry is being received.

## Project Structure

```text
cpp-network-device-monitor/
├── .github/
│   └── workflows/
│       └── ci.yml
├── include/
│   ├── DeviceState.h
│   ├── Telemetry.h
│   ├── ThreadSafeQueue.h
│   └── UdpSocket.h
├── monitor/
│   └── main.cpp
├── simulator/
│   └── main.cpp
├── src/
│   ├── Telemetry.cpp
│   └── UdpSocket.cpp
├── tests/
│   └── tests.cpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```

## Requirements

- Linux
- C++17-compatible compiler such as GCC or Clang
- CMake 3.16 or newer

## Build

Clone the repository:

```bash
git clone git@github.com:Daashk/cpp-network-device-monitor.git
cd cpp-network-device-monitor
```

Configure the project:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

The build creates:

```text
build/device_monitor
build/device_simulator
build/device_tests
```

## Run

Start the monitor:

```bash
./build/device_monitor
```

Open another terminal and start the simulator:

```bash
./build/device_simulator
```

The simulator continuously sends telemetry from multiple virtual devices.

One simulated device intentionally stops transmitting for several cycles to demonstrate offline detection and automatic recovery.

Stop either application with:

```text
Ctrl+C
```

## Tests

Run the complete test suite with CTest:

```bash
ctest --test-dir build --output-on-failure
```

Or run the test executable directly:

```bash
./build/device_tests
```

The tests cover:

- parsing valid telemetry
- rejecting invalid numeric values
- rejecting messages with missing fields
- rejecting malformed telemetry
- device timeout detection
- verifying that recently active devices remain online

## Continuous Integration

GitHub Actions automatically runs on pushes and pull requests.

The CI workflow:

1. checks out the repository
2. configures the project with CMake
3. builds the project
4. runs the test suite with CTest

This provides automatic verification that the project continues to compile and pass its tests after changes.

## Key C++ Concepts Demonstrated

- RAII
- Resource ownership
- Move semantics
- Deleted copy operations
- Multithreading
- Mutex-based synchronization
- Condition variables
- Thread-safe data structures
- STL containers
- Structured bindings
- Exception handling
- UDP socket programming
- Time-based state tracking
- Input parsing and validation
- CMake-based builds
- Automated testing

## Known Limitations

- The sample applications are currently terminated with `Ctrl+C`.
- A production version would use explicit shutdown signalling and join the receiver thread before exit.
- The simulator and monitor currently communicate through the local loopback interface.
- UDP does not guarantee delivery or ordering of packets. A production monitoring system could add additional packet-loss and sequence validation depending on its requirements.

## Possible Future Improvements

- Graceful application shutdown
- Per-device packet-loss statistics
- Sequence-number validation
- Configurable IP address, port, and timeout values
- Structured logging
- Additional unit and integration tests