#include "ESPPreferences.h"

ESPPreferences::ESPPreferences() : preferences_()
{
    preferences_ = Preferences();
}

bool ESPPreferences::begin(const char *name, bool readOnly)
{
    preferences_.begin(name, !readOnly);
    return true;
}

void ESPPreferences::putBool(const char *key, bool value)
{
    preferences_.putBool(key, value);
}
bool ESPPreferences::getBool(const char *key, bool defaultValue)
{
    return preferences_.getBool(key, defaultValue);
}
void ESPPreferences::putInt(const char *key, int value)
{
    preferences_.putInt(key, value);
}
int ESPPreferences::getInt(const char *key, int defaultValue)
{
    return preferences_.getInt(key, defaultValue);
}
void ESPPreferences::putShort(const char *key, short value)
{
    preferences_.putShort(key, value);
}
short ESPPreferences::getShort(const char *key, short defaultValue)
{
    return preferences_.getShort(key, defaultValue);
}
void ESPPreferences::end()
{
    preferences_.end();
}