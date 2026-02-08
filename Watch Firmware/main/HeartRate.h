#ifndef HEARTRATE_H
#define HEARTRATE_H

#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h" // SparkFun library's helper for checkForBeat

// External objects so they can be accessed in the main sketch
extern MAX30105 particleSensor;
extern int beatAvg;

// Function Prototypes
void setupHeartSensor();
int updateHeartRate();

#endif
