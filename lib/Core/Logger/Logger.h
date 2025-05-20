#pragma once

#include <Interfaces/IPreferences.h>
#include <Interfaces/IConsole.h>
#include <cstdarg>
#include <ctime>

constexpr std::size_t MaxLoggerMessageSize = 1024;

enum class LogLevel
{
    Error,
    Warn,
    Info,
    Debug,
    Verbose
};

class Logger
{
public:
    explicit Logger(LogLevel level = LogLevel::Info);
    ~Logger();

    void init(IPreferences *preferences = nullptr, IConsole *console = nullptr);
    LogLevel parseLogLevel(const char *levelStr);
    void setLogLevel(LogLevel level);
    void enableLocalLogging(bool enable);
    void enableRemoteLogging(bool enable, std::time_t expiration = 0, bool persist = false);
    void enablePersistentLocalLogging();
    void log(LogLevel level, const char *format, ...);

    static int customLogWrite(const char *format, std::va_list args);

private:
    IPreferences *preferences_ = nullptr;
    IConsole *console_ = nullptr;
    LogLevel currentLogLevel_ = LogLevel::Info;
    bool localLoggingEnabled_ = false;
    std::time_t remoteLoggingExpiration_ = 0;
    bool isSendingRemoteLog_ = false;

    static Logger *instance_;
};