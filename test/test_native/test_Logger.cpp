#include <doctest/doctest.h>
#include <Core/Logger/Logger.h>
#include <Mocks/MockPreferences.h>
#include <Mocks/MockConsole.h>

#include <memory>
#include <ctime>

TEST_SUITE("Logger")
{
    std::unique_ptr<Logger> logger;
    std::unique_ptr<MockPreferences> prefs;
    std::unique_ptr<MockConsole> console;

    void setupLogger(LogLevel level = LogLevel::Debug)
    {
        prefs.reset(new MockPreferences());
        console.reset(new MockConsole());
        logger.reset(new Logger(level));
        logger->init(prefs.get(), console.get());
    }

    TEST_CASE("Logger filters out messages below current level")
    {
        setupLogger(LogLevel::Error);
        logger->enableLocalLogging(true);
        logger->log(LogLevel::Debug, "This should not appear");
        CHECK(console->getOutput().empty());
    }

    TEST_CASE("Logger logs messages at current level")
    {
        setupLogger(LogLevel::Info);
        logger->enableLocalLogging(true);
        logger->log(LogLevel::Info, "Info level message");
        CHECK(console->getOutput().find("Info level message") != std::string::npos);
    }

    TEST_CASE("Logger disables local logging")
    {
        setupLogger(LogLevel::Info);
        logger->enableLocalLogging(false);
        logger->log(LogLevel::Info, "This should be hidden");
        CHECK(console->getOutput().empty());
    }

    TEST_CASE("Remote logging enabled with expiration in the past doesn't log remotely")
    {
        setupLogger(LogLevel::Info);
        logger->enableLocalLogging(true);
        logger->enableRemoteLogging(true, -5); // force expiration (now + (-5) == past)
        logger->log(LogLevel::Info, "Try logging a remote message");
        auto output = console->getOutput();
        CHECK_MESSAGE(output.find("Remote logging disabled due to expiration.") == std::string::npos,
                      "Message should not have been printed. Output was: " << output);
    }

    TEST_CASE("Logger includes timestamp in output")
    {
        setupLogger(LogLevel::Info);
        logger->enableLocalLogging(true);
        logger->log(LogLevel::Info, "Test with timestamp");
        auto output = console->getOutput();
        CHECK(output.find("UTC") != std::string::npos);
        CHECK(output.find("Test with timestamp") != std::string::npos);
    }

    TEST_CASE("Logger respects remote logging within expiration")
    {
        setupLogger(LogLevel::Info);
        logger->enableLocalLogging(true);
        logger->enableRemoteLogging(true, 60); // expires in future
        logger->log(LogLevel::Info, "Message while remote logging active");
        auto output = console->getOutput();
        CHECK(output.find("Message while remote logging active") != std::string::npos);
    }

    TEST_CASE("Logger stores remote logging expiration when persisted")
    {
        setupLogger(LogLevel::Info);
        logger->enableRemoteLogging(true, 120, true); // persist requested
        // Check that the stored expiration is > 0 (future time)
        CHECK(prefs->getInt("rLogExp") > 0);
    }

    TEST_CASE("Logger disables remote logging and clears expiration when persisted")
    {
        setupLogger(LogLevel::Info);
        logger->enableRemoteLogging(false, 0, true);
        CHECK(prefs->getInt("rLogExp") == 0);
    }

    TEST_CASE("Logger stores log level to preferences")
    {
        setupLogger(LogLevel::Info);
        logger->setLogLevel(LogLevel::Error);
        CHECK(prefs->getShort("logLevel") == static_cast<short>(LogLevel::Error));
    }

    TEST_CASE("Logger disables persistent local logging")
    {
        setupLogger(LogLevel::Info);
        logger->enableLocalLogging(false);
        CHECK(prefs->getBool("localLogging") == false);
    }

    TEST_CASE("Logger enables persistent local logging")
    {
        setupLogger(LogLevel::Info);
        logger->enablePersistentLocalLogging();
        CHECK(prefs->getBool("localLogging") == true);
    }

    TEST_CASE("Logger re-initializes from saved preferences")
    {
        prefs->putBool("localLogging", true);
        prefs->putShort("logLevel", static_cast<short>(LogLevel::Warn));
        prefs->putInt("rLogExp", 0);

        console.reset(new MockConsole());
        logger.reset(new Logger());
        logger->init(prefs.get(), console.get());
        logger->log(LogLevel::Error, "Should be logged");
        logger->log(LogLevel::Info, "Should NOT be logged");

        auto output = console->getOutput();
        CHECK(output.find("Should be logged") != std::string::npos);
        CHECK(output.find("Should NOT be logged") == std::string::npos);
    }
}