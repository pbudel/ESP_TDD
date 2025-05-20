#pragma once

class IPreferences
{
public:
    virtual ~IPreferences() = default;

    virtual bool begin(const char *name, bool readOnly = false) = 0;
    virtual void putBool(const char *key, bool value) = 0;
    virtual bool getBool(const char *key, bool defaultValue = false) = 0;
    virtual void putInt(const char *key, int value) = 0;
    virtual int getInt(const char *key, int defaultValue = 0) = 0;
    virtual void putShort(const char *key, short value) = 0;
    virtual short getShort(const char *key, short defaultValue = 0) = 0;
    virtual void end() = 0;
};