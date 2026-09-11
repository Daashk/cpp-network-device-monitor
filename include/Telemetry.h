#pragma once

#include <string>

struct Telemetry
{
    std::string deviceId;
    double temperature;
    int signalStrength;
    unsigned int sequenceNumber;
};

Telemetry parseTelemetry(const std::string& message);