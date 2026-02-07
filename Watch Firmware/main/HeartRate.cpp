#include <MAX30105.h>
#include <heartRate.h>

// Define the objects declared as 'extern' in the header
MAX30105 particleSensor;
int beatAvg = 0;
long lastBeat = 0;

void setupHeartSensor() {
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Check wiring/power.");
    while (1);
  }

  particleSensor.setup(); // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to save power
  particleSensor.setPulseAmplitudeIR(0x1F);  // Turn IR LED to high for better detection
}

int updateHeartRate() {
  // Check the sensor buffer
  particleSensor.check(); 

  int detectedBPM = 0; // Default: no new value detected

  while (particleSensor.available()) {
    uint32_t irValue = particleSensor.getFIFOIR();

    // Finger detection threshold
    if (irValue > 20000) {
      if (checkForBeat(irValue)) {
        long delta = millis() - lastBeat;
        lastBeat = millis();
        float bpm = 60 / (delta / 1000.0);

        // Filter out noise and unrealistic values
        if (bpm < 255 && bpm > 20) {
          beatAvg = (int)bpm;
          detectedBPM = beatAvg; // Capture the new value to return
        }
      }
    }
    particleSensor.nextSample();
  }
  
  return detectedBPM; // Returns the new BPM or 0 if nothing changed
}