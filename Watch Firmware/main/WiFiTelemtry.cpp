#include "WiFiTelemetry.h"

void setupWiFi() {
  // The Uno R4 Minima uses Serial1 for pins 0 and 1.
  // This is the "bridge" to the Dragino Linux processor.
  Serial1.begin(115200);
  Serial1.print("\n\n");
  
  // Give the Linux system a moment to stabilize after power-up
  delay(2000); 
}

/**
 * Sends the current BPM to a server on your computer.
 * Make sure your computer is connected to the Dragino Wi-Fi
 * and your IP matches the one below.
 */
void uploadBPM(int bpm) {
// Format data as a simple string ending with a newline
  int sensorValue = 100;
  String dataString = "bpm="+String(sensorValue);
  
  // Send to the Yun Shield's Linux processor
  Serial1.println(dataString);
  
  // Debug to your PC via USB
  Serial.print("Sent: ");
  Serial.print(dataString);


  // // 1. We create the URL with the BPM value attached as a query (?bpm=XX)
  // String url = "http://192.168.240.249:8000/?bpm=" + String(bpm);
  
  // // 2. We build the full Linux command. 
  // // We use \" to put quotes around the URL so Linux handles it safely.
  // // We add \n at the end to "Press Enter" on the Linux console.
  // String command = "curl \"" + url + "\"\n";

  // // 3. Send the command to the Dragino Shield
  // Serial1.println(command);
  
  // // Optional: Print to your PC Serial Monitor so you can see what was sent
  // Serial.print("Sending to Server: ");
  // Serial.println(command);
}