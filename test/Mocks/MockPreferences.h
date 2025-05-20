#pragma once

#include <Interfaces/IPreferences.h>
#include <unordered_map>
#include <string>

class MockPreferences : public IPreferences
{
public:
    bool begin(const char *name, bool readOnly = false) override
    {
        namespace_ = name;
        readOnly_ = readOnly;
        return true;
    }

    void end() override
    {
        namespace_.clear();
        readOnly_ = false;
    }

    void putBool(const char *key, bool value) override
    {
        boolStorage[qualifiedKey(key)] = value;
    }

    bool getBool(const char *key, bool defaultValue = false) override
    {
        auto it = boolStorage.find(qualifiedKey(key));
        return it != boolStorage.end() ? it->second : defaultValue;
    }

    void putInt(const char *key, int value) override
    {
        intStorage[qualifiedKey(key)] = value;
    }

    int getInt(const char *key, int defaultValue = 0) override
    {
        auto it = intStorage.find(qualifiedKey(key));
        return it != intStorage.end() ? it->second : defaultValue;
    }

    void putShort(const char *key, short value) override
    {
        shortStorage[qualifiedKey(key)] = value;
    }

    short getShort(const char *key, short defaultValue = 0) override
    {
        auto it = shortStorage.find(qualifiedKey(key));
        return it != shortStorage.end() ? it->second : defaultValue;
    }

private:
    std::string qualifiedKey(const char *key) const
    {
        return namespace_ + "::" + key;
    }

    std::string namespace_;
    bool readOnly_ = false;
    std::unordered_map<std::string, bool> boolStorage;
    std::unordered_map<std::string, int> intStorage;
    std::unordered_map<std::string, short> shortStorage;
};