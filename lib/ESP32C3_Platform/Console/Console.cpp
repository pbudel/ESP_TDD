#include <Arduino.h>
#include "Console.h"
#include <cstdarg>
#include <cstdio>

Console::Console()
{
    // Constructor implementation (if needed)
    Serial.begin(115200); // Initialize Serial communication
}

void Console::printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    Serial.printf(format, args); // Arduino target
    va_end(args);
}

void Console::println(const char *message)
{
    Serial.printf("%s\n", message);
}
