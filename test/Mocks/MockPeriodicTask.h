#include <Interfaces/IPeriodicTask.h>
#include <functional>

class MockPeriodicTask : public IPeriodicTask
{
public:
    bool startCalled = false;
    bool stopCalled = false;
    unsigned long interval = 0;
    std::function<void(void *)> storedHandler = nullptr;

    void startTask() override
    {
        startCalled = true;
    }

    void stopTask() override
    {
        stopCalled = true;
        interval = 0;
        storedHandler = nullptr;
    }

    void setInterval(unsigned long ms_interval) override
    {
        interval = ms_interval;
    }

    unsigned long getInterval() const override
    {
        return interval;
    }

    void setHandler(std::function<void(void *)> task_handler) override
    {
        storedHandler = task_handler;
    }

    ~MockPeriodicTask() override = default;
};