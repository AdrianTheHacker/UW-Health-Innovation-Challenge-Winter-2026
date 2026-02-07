#include "HeartrateMonitor.hpp"
#include "heartRate.h" // Includes the SparkFun beat detection logic

HeartrateMonitor::HeartrateMonitor() {
    found = false;
    lastBeatTime = 0;
    current_bpm = 0;
}

int HeartrateMonitor::begin() {
    // Initialize sensor; return SUCCESS (0) or FAILURE (-1)
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        found = false;
        return FAILURE;
    }

    found = true;
    particleSensor.setup(); 
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);
    return SUCCESS;
}

int HeartrateMonitor::get_pulse(int &value) {
    if (!found) return FAILURE;

    // Read the IR value - this is non-blocking in the SparkFun library
    long irValue = particleSensor.getIR();

    // Finger detection threshold
    if (irValue < 50000) {
        value = 0; 
        return SUCCESS;
    }

    // checkForBeat() is a non-blocking mathematical algorithm
    if (checkForBeat(irValue)) {
        long delta = millis() - lastBeatTime;
        lastBeatTime = millis();

        // Calculate BPM
        float beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            current_bpm = (int)beatsPerMinute;
        }
    }

    value = current_bpm;
    return SUCCESS;
}