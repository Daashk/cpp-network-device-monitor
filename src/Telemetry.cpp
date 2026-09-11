#include "Telemetry.h"

#include <sstream>
#include <stdexcept>

Telemetry parseTelemetry(const std::string& message)
{
    std::stringstream stream(message);
    std::string part;

    Telemetry telemetry;

    if (!std::getline(stream, telemetry.deviceId, '|'))
        throw std::runtime_error("Missing device ID");

    if (!std::getline(stream, part, '|'))
        throw std::runtime_error("Missing temperature");

    telemetry.temperature = std::stod(part);

    if (!std::getline(stream, part, '|'))
        throw std::runtime_error("Missing signal strength");

    telemetry.signalStrength = std::stoi(part);

    if (!std::getline(stream, part, '|'))
        throw std::runtime_error("Missing sequence number");

    telemetry.sequenceNumber =
        static_cast<unsigned int>(std::stoul(part));

    return telemetry;
}