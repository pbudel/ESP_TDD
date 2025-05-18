#include <Interfaces/ILed.h>

class MockLed : public ILed
{
public:
    bool onCalled = false;
    bool offCalled = false;

    void turnOn() override { onCalled = true; }
    void turnOff() override { offCalled = true; }
};