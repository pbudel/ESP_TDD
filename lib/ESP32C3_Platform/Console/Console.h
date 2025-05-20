#pragma once
#include <Interfaces/IConsole.h>

class Console : public IConsole
{
public:
    Console();
    void printf(const char *format, ...) override;
    void println(const char *message) override;
    ~Console() override = default;
};
