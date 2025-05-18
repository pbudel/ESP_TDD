#pragma once
#include <Interfaces/ILed.h>
#include <Interfaces/IPeriodicTask.h>
#include <functional>

class Blinker
{
public:
    Blinker(ILed &led, unsigned long intervalMs = 500);
    virtual ~Blinker() = default;

    void start();
    void stop();
    bool isRunning() const;
    void setInterval(unsigned long intervalMs);

protected:
    virtual std::unique_ptr<IPeriodicTask> createTask(); // Factory overridabile
    std::unique_ptr<IPeriodicTask> task;

private:
    ILed &led;
    unsigned long intervalMs;
    bool ledState = false;
    bool running = false;
    std::function<void(void *)> _loopFunction;
};