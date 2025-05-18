#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <memory>

#include <Mocks/MockLed.h>
#include <Mocks/MockPeriodicTask.h>
#include <Core/Blinker/Blinker.h>

class TestableBlinker : public Blinker
{
public:
    TestableBlinker(ILed &led, std::unique_ptr<IPeriodicTask> mockTask, unsigned long intervalMs = 500)
        : Blinker(led, intervalMs)
    {
        this->task = std::move(mockTask);
    }

    ~TestableBlinker() override = default;
};

TEST_CASE("Blinker start configures periodic task and toggles LED")
{
    auto mockTask = std::unique_ptr<MockPeriodicTask>(new MockPeriodicTask());
    auto *taskPtr = mockTask.get(); // per accedere a storedHandler dopo start()

    MockLed led;
    TestableBlinker blinker(led, std::move(mockTask), 1000);

    blinker.start();

    // Verifica che il task sia stato configurato
    CHECK(taskPtr->startCalled);
    CHECK(taskPtr->interval == 1000);
    REQUIRE(taskPtr->storedHandler); // handler assegnato

    // Simulazione chiamata periodica
    taskPtr->storedHandler(nullptr);
    bool toggled = led.onCalled || led.offCalled;
    CHECK_MESSAGE(toggled, "LED should have been toggled on or off");
}

TEST_CASE("Blinker toggles LED state on repeated handler calls")
{
    auto mockTask = std::unique_ptr<MockPeriodicTask>(new MockPeriodicTask());
    auto *taskPtr = mockTask.get();

    MockLed led;
    TestableBlinker blinker(led, std::move(mockTask), 500);

    blinker.start();
    REQUIRE(taskPtr->storedHandler);

    // Prima chiamata accende
    taskPtr->storedHandler(nullptr);
    CHECK(led.onCalled);
    led.onCalled = false;

    // Seconda chiamata spegne
    taskPtr->storedHandler(nullptr);
    CHECK(led.offCalled);
    led.offCalled = false;

    // Terza chiamata accende di nuovo
    taskPtr->storedHandler(nullptr);
    CHECK(led.onCalled);
}