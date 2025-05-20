#include <doctest/doctest.h>
#include <memory>

#include <Mocks/MockLed.h>
#include <Mocks/MockPeriodicTask.h>
#include <Core/Blinker/Blinker.h>

class TestableBlinker : public Blinker
{
public:
    TestableBlinker(ILed &led, unsigned long intervalMs = 500, std::unique_ptr<IPeriodicTask> mockTask = nullptr)
        : Blinker(led, intervalMs)
    {
        this->task = std::move(mockTask);
    }

    ~TestableBlinker() override = default;
};

TEST_CASE("Blinker start configures periodic task and toggles LED")
{
    auto mockTask = std::unique_ptr<MockPeriodicTask>(new MockPeriodicTask());
    auto *taskPtr = mockTask.get();

    MockLed led(1);
    TestableBlinker blinker(led, 1000, std::move(mockTask));

    blinker.start();

    REQUIRE_MESSAGE(taskPtr->storedHandler, "Handler was not set before use");

    // Check that the task has been configured
    CHECK(taskPtr->startCalled);
    CHECK(taskPtr->interval == 1000);
    REQUIRE(taskPtr->storedHandler); // handler assigned

    // Simulate periodic call
    taskPtr->storedHandler(nullptr);
    bool toggled = led.onCalled || led.offCalled;
    CHECK_MESSAGE(toggled, "LED should have been toggled on or off");
}

TEST_CASE("Blinker toggles LED state on repeated handler calls")
{
    auto mockTask = std::unique_ptr<MockPeriodicTask>(new MockPeriodicTask());
    auto *taskPtr = mockTask.get();

    MockLed led(1);
    TestableBlinker blinker(led, 500, std::move(mockTask));

    blinker.start();
    REQUIRE_MESSAGE(taskPtr->storedHandler, "Handler was not set before use");
    taskPtr->storedHandler(nullptr); // sicuro
    REQUIRE(taskPtr->storedHandler);

    // First call turns on
    taskPtr->storedHandler(nullptr);
    CHECK(led.onCalled);
    led.onCalled = false;

    // Second call turns off
    taskPtr->storedHandler(nullptr);
    CHECK(led.offCalled);
    led.offCalled = false;

    // Third call turns on again
    taskPtr->storedHandler(nullptr);
    CHECK(led.onCalled);
}