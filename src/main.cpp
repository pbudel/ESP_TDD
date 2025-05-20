#include <Arduino.h>
#include <Core/Blinker/Blinker.h>
#include <Core/Logger/Logger.h>

#include <ESP32C3_Platform/Led/Led.h>
#include <ESP32C3_Platform/PeriodicTask/PeriodicTask.h>
#include <ESP32C3_Platform/RegisterLogger/RegisterLogger.h>
#include <ESP32C3_Platform/Console/Console.h>
#include <ESP32C3_Platform/Preferences/ESPPreferences.h>

Led led(8); // GPIO 8 for ESP32-C3-Super Mini
auto factory = []()
{
  return std::unique_ptr<IPeriodicTask>(new PeriodicTask()); // or your actual class name
};
Blinker blinker(led, 50, factory);
Console console;
Logger logger;
ESPPreferences preferences;

void setup()
{
  logger.init(&preferences, &console);
  logger.enableLocalLogging(true);
  logger.setLogLevel(LogLevel::Info);
  logger.log(LogLevel::Info, "Starting setup...");

  registerEspLogger();

  blinker.start();

  logger.log(LogLevel::Info, "Blinker started");
}

void loop()
{
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  logger.log(LogLevel::Info, "Looping...");
}