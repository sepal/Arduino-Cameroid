
#include <ByteUtilities.h>
#include <OSCMessage.h>
#include <SLIP.h>
#include <SLIPSerialUsb.h>                              

#include "Cameroid.h"



Cameroid cam(7, 6, 5, 4);

void setup() {
  Serial.begin(115200);
  cam.setup();
}

void loop() {
  cam.loop();
}
