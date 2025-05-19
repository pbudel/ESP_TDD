#include "Blinker.h"
#include <cassert>

Blinker::Blinker(ILed &led, unsigned long intervalMs, TaskFactory factory)
    : led(led), intervalMs(intervalMs), factory(factory), task(nullptr) {}

std::unique_ptr<IPeriodicTask> Blinker::createTask()
{
    if (factory)
        return factory();
    return nullptr;
}

void Blinker::start()
{
    if (running)
        return;

    if (!task && factory)
        task = factory();

    assert(task.get() != nullptr && "Task must be created before starting the Blinker");
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