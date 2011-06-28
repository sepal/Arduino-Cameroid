/**
 * This file is part of Cameroid.
 * Cameroid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Cameroid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with Cameroid.  If not, see <http://www.gnu.org/licenses/>. at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "Masteroid.h"


Masteroid::Masteroid() {}

void Masteroid::setup()
{
  init();
}

void Masteroid::loop()
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

void Masteroid::sendOSC(OSC::Message *msg)
{
  int len = msg->length();
  unsigned char *packet = (unsigned char *) malloc(len);
  msg->getPacket(packet);
  slip.sendPacket(packet, len);
  free(packet);
}
