#pragma once
#include <Interfaces/ILed.h>

class Led : public ILed
{
public:
    explicit Led(uint8_t pin);
    void turnOn() override;
    void turnOff() override;
    ~Led() override = default;

private:
    uint8_t pin;
};