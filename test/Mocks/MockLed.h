#include <Interfaces/ILed.h>

class MockLed : public ILed
{
public:
    MockLed(uint8_t pin) : ILed()
    {
        // Constructor implementation (if needed)
        // Not needed for this mock class
    }
    bool onCalled = false;
    bool offCalled = false;

    void turnOn() override { onCalled = true; }
    void turnOff() override { offCalled = true; }
};