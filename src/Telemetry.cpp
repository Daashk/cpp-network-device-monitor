#include "Telemetry.h"

#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
double parseDouble(
    const std::string& text,
    const std::string& fieldName)
{
    std::size_t position = 0;

    try
    {
        double value = std::stod(text, &position);

        if (position != text.size())
        {
            throw std::runtime_error(
                "Invalid " + fieldName
            );
        }

        return value;
    }
    catch (const std::exception&)
    {
        throw std::runtime_error(
            "Invalid " + fieldName
        );
    }
}

int parseInt(
    const std::string& text,
    const std::string& fieldName)
{
    std::size_t position = 0;

    try
    {
        int value = std::stoi(text, &position);

        if (position != text.size())
        {
            throw std::runtime_error(
                "Invalid " + fieldName
            );
        }

        return value;
    }
    catch (const std::exception&)
    {
        throw std::runtime_error(
            "Invalid " + fieldName
        );
    }
}

unsigned int parseUnsigned(
    const std::string& text,
    const std::string& fieldName)
{
    if (text.empty() || text.front() == '-')
    {
        throw std::runtime_error(
            "Invalid " + fieldName
        );
    }

    std::size_t position = 0;

    try
    {
        unsigned long value =
            std::stoul(text, &position);

        if (position != text.size())
        {
            throw std::runtime_error(
                "Invalid " + fieldName
            );
        }

        return static_cast<unsigned int>(value);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error(
            "Invalid " + fieldName
        );
    }
}
}

Telemetry parseTelemetry(const std::string& message)
{
    std::stringstream stream(message);
    std::string part;

    Telemetry telemetry;

    if (!std::getline(stream, telemetry.deviceId, '|') ||
        telemetry.deviceId.empty())
    {
        throw std::runtime_error("Missing device ID");
    }

    if (!std::getline(stream, part, '|'))
    {
        throw std::runtime_error("Missing temperature");
    }

    telemetry.temperature =
        parseDouble(part, "temperature");

    if (!std::getline(stream, part, '|'))
    {
        throw std::runtime_error(
            "Missing signal strength"
        );
    }

    telemetry.signalStrength =
        parseInt(part, "signal strength");

    if (!std::getline(stream, part, '|'))
    {
        throw std::runtime_error(
            "Missing sequence number"
        );
    }

    telemetry.sequenceNumber =
        parseUnsigned(part, "sequence number");

    if (std::getline(stream, part, '|'))
    {
        throw std::runtime_error(
            "Too many telemetry fields"
        );
    }

    return telemetry;
}