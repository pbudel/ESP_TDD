#include <Arduino.h>
#include <Core/Blinker/Blinker.h>

#include <ESP32C3_Platform/Led/Led.h>
#include <ESP32C3_Platform/PeriodicTask/PeriodicTask.h>

Led led(8);
auto factory = []()
{
  return std::unique_ptr<IPeriodicTask>(new PeriodicTask()); // or your actual class name
};
Blinker blinker(led, 50, factory);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  blinker.start();

  Serial.println("Blinker started");
}

void loop()
{
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  Serial.println("Looping...");
}