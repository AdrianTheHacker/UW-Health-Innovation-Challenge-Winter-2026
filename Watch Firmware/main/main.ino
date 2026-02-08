#include <TaskScheduler.h>
#include "HeartRate.h"
#include "SafeQueue.h"
#include "WiFiTelemetry.h"

// Callback function prototypes
void hardware_thread();
void telemetry_thread();

// Scheduler and Queue
Scheduler runner;
SafeQueue heartQueue(10);

// Tasks:
// Poll hardware every 20ms (50Hz) - NO PRINTING ALLOWED HERE
Task tHardware(20, TASK_FOREVER, &hardware_thread, &runner, true);
// Update screen/serial every 1000ms (1Hz)
Task tTelemetry(1000, TASK_FOREVER, &telemetry_thread, &runner, true);

void setup() {
  // Increase speed to 115200 to prevent buffer bottlenecks
  Serial.begin(115200);
  while (!Serial)
    ;

  setupHeartSensor();
  Serial.println("--- System Started ---");
}

void loop() {
  runner.execute();
}

void hardware_thread() {
  // IMPORTANT: No Serial.println("Hello") or "Hardware" here!
  int currentBPM = updateHeartRate();

  if (currentBPM > 0) {
    heartQueue.push(currentBPM);
  }
}

void telemetry_thread() {
  // Unfinished

  int latestBPM = 0;

  // Always print the header so you know the task is running
  Serial.println("\n--- WATCH STATUS ---");

  if (heartQueue.pop(latestBPM)) {
    Serial.print("BPM: ");
    Serial.println(latestBPM);

    // Upload to WiFi
    uploadBPM(latestBPM);
  } else {
    // This runs when the queue is empty (no finger)
    Serial.println("Pulse: Searching... (Check finger placement)");
  }
}