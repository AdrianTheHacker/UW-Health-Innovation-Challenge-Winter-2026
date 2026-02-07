#ifndef HEARTRATE_MONITOR_H
#define HEARTRATE_MONITOR_H

#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"

class HeartrateMonitor {
    private:
        MAX30105 particleSensor;
        bool found;

        // Variables for the non-blocking beat detection
        long lastBeatTime; 
        int current_bpm;

        // Constants for success/failure
        static const int SUCCESS_VAL = 0;
        static const int FAILURE_VAL = -1;

    public:
        // Use the class constants for return values
        const int SUCCESS = SUCCESS_VAL;
        const int FAILURE = FAILURE_VAL;

        HeartrateMonitor();

        /**
         * Initializes the sensor. Call this in setup().
         * @return SUCCESS if sensor is found, FAILURE otherwise.
         */
        int begin();

        /**
         * Non-blocking pulse update. 
         * Should be called frequently (e.g., inside hardware_thread).
         * @param value Reference to int where the BPM will be stored.
         * @return SUCCESS if sensor is operational.
         */
        int get_pulse(int &value);
};

#endif