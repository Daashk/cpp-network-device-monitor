#include "Telemetry.h"
#include "DeviceState.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

void expect(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(
            "Test failed: " + message
        );
    }
}

void testValidTelemetry()
{
    Telemetry telemetry =
        parseTelemetry("DEVICE01|42.5|-63|12");

    expect(
        telemetry.deviceId == "DEVICE01",
        "device ID"
    );

    expect(
        telemetry.temperature == 42.5,
        "temperature"
    );

    expect(
        telemetry.signalStrength == -63,
        "signal strength"
    );

    expect(
        telemetry.sequenceNumber == 12,
        "sequence number"
    );
}

void testInvalidTelemetry()
{
    bool exceptionThrown = false;

    try
    {
        parseTelemetry("DEVICE01|invalid|-63|12");
    }
    catch (const std::exception&)
    {
        exceptionThrown = true;
    }

    expect(
        exceptionThrown,
        "invalid telemetry should throw"
    );
}

void testMissingFields()
{
    bool exceptionThrown = false;

    try
    {
        parseTelemetry("DEVICE01|42.5");
    }
    catch (const std::exception&)
    {
        exceptionThrown = true;
    }

    expect(
        exceptionThrown,
        "missing fields should throw"
    );
}

void testDeviceOffline()
{
    using namespace std::chrono;

    auto now = steady_clock::now();

    DeviceState state{
        Telemetry{"DEVICE01", 42.5, -63, 1},
        now - seconds(5),
        true
    };

    expect(
        isOffline(state, now, seconds(3)),
        "device should be offline"
    );
}

void testDeviceStillOnline()
{
    using namespace std::chrono;

    auto now = steady_clock::now();

    DeviceState state{
        Telemetry{"DEVICE01", 42.5, -63, 1},
        now - seconds(1),
        true
    };

    expect(
        !isOffline(state, now, seconds(3)),
        "device should still be online"
    );
}

void testTrailingCharacters()
{
    bool exceptionThrown = false;

    try
    {
        parseTelemetry(
            "DEVICE01|42.5abc|-63|12"
        );
    }
    catch (const std::exception&)
    {
        exceptionThrown = true;
    }

    expect(
        exceptionThrown,
        "trailing characters should be rejected"
    );
}

void testExtraFields()
{
    bool exceptionThrown = false;

    try
    {
        parseTelemetry(
            "DEVICE01|42.5|-63|12|EXTRA"
        );
    }
    catch (const std::exception&)
    {
        exceptionThrown = true;
    }

    expect(
        exceptionThrown,
        "extra fields should be rejected"
    );
}

int main()
{
    try
    {
        testValidTelemetry();
        testInvalidTelemetry();
        testMissingFields();
        testDeviceOffline();
        testDeviceStillOnline();
        testTrailingCharacters();
        testExtraFields();

        std::cout << "All tests passed!\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}