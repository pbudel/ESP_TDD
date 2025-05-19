#pragma once
#include <Interfaces/ILed.h>
#include <Interfaces/IPeriodicTask.h>
#include <memory>
#include <functional>

using TaskFactory = std::function<std::unique_ptr<IPeriodicTask>()>;

class Blinker
{
public:
    Blinker(ILed &led, unsigned long intervalMs = 500, TaskFactory taskFactory = nullptr);
    virtual ~Blinker() = default;

    void start();
    void stop();
    bool isRunning() const;
    void setInterval(unsigned long intervalMs);

protected:
    virtual std::unique_ptr<IPeriodicTask> createTask(); // Overridable method for task creation
    std::unique_ptr<IPeriodicTask> task;

private:
    ILed &led;
    TaskFactory factory;
    unsigned long intervalMs;
    bool ledState = false;
    bool running = false;
    std::function<void(void *)> _loopFunction;
};