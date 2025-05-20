#pragma once
#include <Interfaces/IPreferences.h>
#include <Preferences.h>

class ESPPreferences : public IPreferences
{
public:
    ESPPreferences();
    bool begin(const char *name, bool readOnly = false) override;
    void putBool(const char *key, bool value) override;
    bool getBool(const char *key, bool defaultValue = false) override;
    void putInt(const char *key, int value) override;
    int getInt(const char *key, int defaultValue = 0) override;
    void putShort(const char *key, short value) override;
    short getShort(const char *key, short defaultValue = 0) override;
    void end() override;
    ~ESPPreferences() override = default;

private:
    Preferences preferences_;
};
