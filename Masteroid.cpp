
#include "Masteroid.h"


Masteroid::Masteroid() {}

Masteroid::setup()
{
  init();
}

Masteroid::loop()
{
  if (slip.packetAvailable())
  {
    int len = slip.getPacketLen();
    unsigned char *packet = (unsigned char *) malloc(len);
    slip.retrievePacket(packet, len);
    OSC::Message msg;
    msg.setPacket(packet, len);
    free(packet);
    eventOSC(&msg);
  }
  update();
}

Masteroid::sendOSC(OSC::Message *msg)
{
  int len = msg->length();
  unsigned char *packet = (unsigned char *) malloc(len);
  msg->getPacket(packet);
  slip.sendPacket(packet, len);
  free(packet);
}
