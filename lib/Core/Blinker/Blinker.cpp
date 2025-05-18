#include "Blinker.h"
#include <cassert>

Blinker::Blinker(ILed &led, unsigned long intervalMs)
    : led(led), intervalMs(intervalMs), task(createTask()) {}

std::unique_ptr<IPeriodicTask> Blinker::createTask()
{
    // Non viene mai usato in test, ma serve per la vtable
    return nullptr;
}

void Blinker::start()
{
    if (running)
        return;
    assert(task && "task is null — did you forget to override createTask()?");
    running = true;

    _loopFunction = [this](void *)
    {
        ledState = !ledState;
        ledState ? led.turnOn() : led.turnOff();
    };

    task->setInterval(intervalMs);
    task->setHandler(_loopFunction);
    task->startTask();
}

void Blinker::stop()
{
    if (!running)
        return;
    task->stopTask();
    running = false;
}

bool Blinker::isRunning() const
{
    return running;
}

void Blinker::setInterval(unsigned long intervalMs)
{
    this->intervalMs = intervalMs;
    if (task)
    {
        task->setInterval(intervalMs);
    }
}