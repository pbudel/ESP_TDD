// Collection of reusable functions and utilities

#include "Core/Core.h"

std::string urlEncode(const std::string &msg)
{
    std::ostringstream encoded;
    for (unsigned char c : msg)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            encoded << c;
        }
        else
        {
            encoded << '%' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
    }
    return encoded.str();
}