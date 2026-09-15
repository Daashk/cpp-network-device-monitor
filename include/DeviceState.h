#pragma once

#include "Telemetry.h"
#include <chrono>

struct DeviceState
{
    Telemetry telemetry;
    std::chrono::steady_clock::time_point lastSeen;
    bool online = true;
};