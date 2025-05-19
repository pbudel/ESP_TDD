#include <Arduino.h>
#include "PeriodicTask.h"

void PeriodicTask::_handleTask(PeriodicTask *instance)
{
    while (instance->running)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - instance->lastExecution >= instance->intervalMs)
        {
            instance->lastExecution = currentMillis;
            if (instance->handler)
                instance->handler(instance);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS); // Yield to other tasks
    }
}

void PeriodicTask::startTask()
{
    // Check if the task is already running and handlers is set
    if (running || handler == nullptr)
        return;

    // Start FreeRTOS task to run the handler periodically
    xTaskCreate((TaskFunction_t)&_handleTask, "PeriodicTask", 4 * configMINIMAL_STACK_SIZE, this, 1, &taskHandle);
    running = true;
}

void PeriodicTask::stopTask()
{
    running = false;
}

void PeriodicTask::setInterval(unsigned long intervalMs)
{
    this->intervalMs = intervalMs;
}

unsigned long PeriodicTask::getInterval() const
{
    return intervalMs;
}

void PeriodicTask::setHandler(std::function<void(void *)> handler)
{
    this->handler = handler;
}