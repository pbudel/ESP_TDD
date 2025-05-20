

#pragma once

#include <Interfaces/IConsole.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdarg>
#include <cstdio>

class MockConsole : public IConsole
{
public:
    void println(const char *message) override
    {
        buffer_.emplace_back(std::string(message) + "\n");
    }

    void printf(const char *format, ...) override
    {
        char formatted[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(formatted, sizeof(formatted), format, args);
        va_end(args);
        buffer_.emplace_back(formatted);
    }

    std::string getOutput() const
    {
        std::ostringstream oss;
        for (const auto &entry : buffer_)
        {
            oss << entry;
        }
        return oss.str();
    }

    void clear()
    {
        buffer_.clear();
    }

private:
    std::vector<std::string> buffer_;
};