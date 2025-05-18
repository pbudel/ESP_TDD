#pragma once

class ILed
{
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual ~ILed() = default;
};