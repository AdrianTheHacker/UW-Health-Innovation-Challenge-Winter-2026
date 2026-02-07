#include <Arduino.h>
#include <ThreadController.h>
#include <Thread.h>

void setup() {
  ThreadController controller = ThreadController();

  Thread* hardware = new Thread();
  hardware->enabled = true;
  hardware->setInterval(100);

  Thread* telemetry = new Thread();
  telemetry->enabled = true;
  telemetry->setInterval(1000);

  controller.add(hardware);
  controller.add(telemetry);
}

void loop() {

}
