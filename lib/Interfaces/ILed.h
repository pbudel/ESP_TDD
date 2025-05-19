#pragma once
#include <stdint.h> // Style C compatibility

class ILed
{
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual ~ILed() = default;

protected:
    uint8_t pin;
};