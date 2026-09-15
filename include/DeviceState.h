#pragma once

#include "Telemetry.h"
#include <chrono>

struct DeviceState
{
    Telemetry telemetry;
    std::chrono::steady_clock::time_point lastSeen;
    bool online = true;
};

inline bool isOffline(
    const DeviceState& state,
    std::chrono::steady_clock::time_point now,
    std::chrono::steady_clock::duration timeout)
{
    return now - state.lastSeen > timeout;
}