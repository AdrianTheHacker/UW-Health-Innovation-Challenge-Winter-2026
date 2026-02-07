#include "WiFiTelemetry.h"

void setupWiFi() {
  // Start Serial1 at 57600 (default for Yun Shield Linux console)
  // This replaces Bridge.begin()
  Serial1.begin(57600); 
}

void uploadBPM(int bpm) {
  // We send a raw string that the Linux side can parse
  // Sending as a formatted command that Linux can execute
  Serial1.print("curl http://10.36.121.198:5000/device-data/" + String(bpm));
  Serial1.print(bpm);
  Serial1.println("\""); 
}
