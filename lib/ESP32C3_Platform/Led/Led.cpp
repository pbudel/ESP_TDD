#include <Arduino.h>
#include "Led.h"

Led::Led(uint8_t pin) : pin(pin)
{
    pinMode(pin, OUTPUT);
}

void Led::turnOn()
{
    digitalWrite(pin, LOW);
}

void Led::turnOff()
{
    digitalWrite(pin, HIGH);
}