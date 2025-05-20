#include "Logger.h"
#include <sstream>
#include <array>
#include <string>
#include <algorithm>
#include <cctype>

Logger *Logger::instance_ = nullptr; // Initialize the static instance pointer

// Constructor to initialize the logger with a log level and CloudAgent reference
Logger::Logger(LogLevel level) : remoteLoggingExpiration_(0)
{
    currentLogLevel_ = level;
    localLoggingEnabled_ = false; // Default to local logging disabled

    // Set the static instance pointer to this instance
    instance_ = this;
}

Logger::~Logger()
{
    preferences_ = nullptr;
    console_ = nullptr;
    instance_ = nullptr;
}

// Static log handler method (no log level and tag here)
int Logger::customLogWrite(const char *format, va_list args)
{
    if (instance_ != nullptr) // Ensure we have a valid instance of Logger
    {
        char buffer[512];
        vsnprintf(buffer, sizeof(buffer), format, args);

        // Assume INFO level for now, as we cannot get the exact level here
        instance_->log(LogLevel::Info, "%s", buffer);
    }

    return vprintf(format, args);
}

void Logger::init(IPreferences *preferences, IConsole *console)
{
    preferences_ = preferences; // Use the provided Preferences object
    console_ = console;         // Use the provided Console object

    // Initialize NVS for persistent storage -- logger namespace
    if (!preferences_->begin("logger", false))
    {
        console_->println("Failed to initialize preferences");
        return;
    }

    localLoggingEnabled_ = preferences_->getBool("localLogging", false); // Default to local logging disabled
    currentLogLevel_ = static_cast<LogLevel>(preferences_->getShort("logLevel", static_cast<short>(LogLevel::Info)));

    // Load remote logging expiration value
    remoteLoggingExpiration_ = static_cast<time_t>(preferences_->getInt("rLogExp", 0)); // Default to 0 (disabled)

    time_t now = time(NULL);
    if (remoteLoggingExpiration_ != -1 && remoteLoggingExpiration_ != 0 && now >= remoteLoggingExpiration_)
    {
        // If the expiration has passed, disable remote logging
        remoteLoggingExpiration_ = 0;
        preferences_->putInt("rLogExp", 0);
    }

    log(LogLevel::Info, "Logger initialized. Local logging: %s, Remote logging expiration: %ld",
        localLoggingEnabled_ ? "enabled" : "disabled",
        remoteLoggingExpiration_);
}

// Enable or disable remote logging with optional expiration timestamp, persist
void Logger::enableRemoteLogging(bool enable, time_t expiration, bool persist)
{
    if (enable)
    {
        if (expiration == -1)
        {
            remoteLoggingExpiration_ = -1; // Always on
        }
        else if (expiration > 0)
        {
            remoteLoggingExpiration_ = time(NULL) + expiration; // Set expiration
        }
        else
        {
            remoteLoggingExpiration_ = 0; // Remote logging disabled
        }

        if (persist)
        {
            preferences_->putInt("rLogExp", static_cast<int32_t>(remoteLoggingExpiration_)); // Persist expiration
        }
    }
    else
    {
        remoteLoggingExpiration_ = 0; // Disable remote logging
        if (persist)
        {
            preferences_->putInt("rLogExp", 0); // Persist that it's disabled
        }
    }
}

LogLevel Logger::parseLogLevel(const char *levelStr)
{
    std::string level(levelStr);
    std::transform(level.begin(), level.end(), level.begin(), [](unsigned char c)
                   { return std::toupper(c); });

    if (level == "ERROR")
        return LogLevel::Error;
    if (level == "WARN")
        return LogLevel::Warn;
    if (level == "INFO")
        return LogLevel::Info;
    if (level == "DEBUG")
        return LogLevel::Debug;
    if (level == "VERBOSE")
        return LogLevel::Verbose;

    return LogLevel::Info;
}

// Set the current log level
void Logger::setLogLevel(LogLevel level)
{
    currentLogLevel_ = level;
    preferences_->putShort("logLevel", static_cast<short>(level)); // Update NVS
}

// Enable or disable local logging (Console output)
void Logger::enableLocalLogging(bool enable)
{
    localLoggingEnabled_ = enable;
    if (!enable)
    {
        preferences_->putBool("localLogging", false); // Update NVS if disabled
    }
}

void Logger::enablePersistentLocalLogging()
{
    localLoggingEnabled_ = true;
    preferences_->putBool("localLogging", true); // Store in NVS to persist across reboots
}

// Log function with variable arguments
void Logger::log(LogLevel level, const char *format, ...)
{
    if (level > currentLogLevel_)
    {
        return;
    }

    // Use std::array for stack buffer, and std::ostringstream for formatting
    va_list args;
    va_start(args, format);
    std::array<char, MaxLoggerMessageSize> logBuffer{};
    vsnprintf(logBuffer.data(), logBuffer.size(), format, args);
    va_end(args);

    // Handle local logging
    if (localLoggingEnabled_ && console_)
    {
        // Get current time as UTC using std::chrono and std::time
        auto now = std::time(nullptr);
        std::tm timeInfo{};
        gmtime_r(&now, &timeInfo);
        char timeString[64];
        std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeInfo);

        std::ostringstream oss;
        oss << "[" << timeString << " UTC] " << logBuffer.data();
        console_->println(oss.str().c_str());
    }

    // Handle remote logging
    auto now = std::time(nullptr);
    if (remoteLoggingExpiration_ == -1 || now < remoteLoggingExpiration_)
    {
        // Placeholder: _sendLogToCloud(level, logBuffer.data());
    }
    else if (remoteLoggingExpiration_ != 0 && now >= remoteLoggingExpiration_)
    {
        if (!isSendingRemoteLog_)
        {
            isSendingRemoteLog_ = true;
            if (console_)
            {
                auto nowTime = std::time(nullptr);
                std::tm timeInfo{};
                gmtime_r(&nowTime, &timeInfo);
                char timeString[64];
                std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeInfo);

                std::ostringstream oss;
                oss << "[" << timeString << " UTC] Remote logging disabled due to expiration.\n";
                console_->println(oss.str().c_str());
            }
            isSendingRemoteLog_ = false;
        }
    }
}