#pragma once
#include <Interfaces/IPeriodicTask.h>

class PeriodicTask : public IPeriodicTask
{
public:
    explicit PeriodicTask() {};
    void startTask() override;
    void stopTask() override;
    void setInterval(unsigned long intervalMs) override;
    unsigned long getInterval() const override;
    void setHandler(std::function<void(void *)> handler) override;
    ~PeriodicTask() override = default;

private:
    TaskHandle_t taskHandle = nullptr;
    static void _handleTask(PeriodicTask *instance);
    unsigned long intervalMs;
    unsigned long lastExecution;
    bool running = false;
    std::function<void(void *)> handler = nullptr;
};
