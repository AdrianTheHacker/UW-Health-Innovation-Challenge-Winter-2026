/*
  SDA is white
  SCL is yellow
*/

#include <Arduino.h>
#include <ThreadController.h>
#include <Thread.h>
#include "./../lib/MutexBlockedQueue/MutexBlockedQueue.hpp"
#include <Wire.h>
#include "MAX30105.h"
#include <Serial.h>
#include "./../lib/HeartrateMonitor/HeartrateMonitor.hpp"

void hardware_thread();
void telemetry_thread();

MutexBlockedQueue queue(5);
HeartrateMonitor heartrateMonitor;

void setup()
{
  heartrateMonitor.begin();
  Serial.begin(9600);
  Serial.println("Hello, world!");

  // ThreadController controller = ThreadController();

  // Thread *hardware = new Thread();
  // hardware->enabled = true;
  // hardware->setInterval(100);
  // hardware->onRun(hardware_thread);
  // controller.add(hardware);

  // Thread *telemetry = new Thread();
  // telemetry->enabled = true;
  // telemetry->setInterval(1000);
  // telemetry->onRun(telemetry_thread);
  // controller.add(telemetry);

  // controller.run();
}

void loop() {}

void hardware_thread()
{
  int value = 0;
  heartrateMonitor.get_pulse(value);

  Serial.print("Pulse: ");
  Serial.println(value);
}

void telemetry_thread()
{
}
