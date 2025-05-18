#pragma once

#include <functional>
class IPeriodicTask
{
public:
    virtual void startTask() = 0;
    virtual void stopTask() = 0;
    virtual void setInterval(unsigned long ms_interval) = 0;
    virtual unsigned long getInterval() const = 0;
    virtual void setHandler(std::function<void(void *)> task_handler) = 0;
    virtual ~IPeriodicTask() = default;
};