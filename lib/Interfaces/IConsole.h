#pragma once

#include <cstdarg>

class IConsole
{
public:
    virtual ~IConsole() = default;

    virtual void println(const char *message) = 0;
    virtual void printf(const char *format, ...) = 0;
};