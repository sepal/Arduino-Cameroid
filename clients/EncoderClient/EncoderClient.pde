

/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.x-0.x
 *
 * @section LICENSE
 *
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
 * along with Cameroid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * This client for the cameroid uses two Rotary encoders. Currently
 * only one encoder is used. There are are 3 additional buttons, 
 * one is the zoom reset, the second is for entering and exiting the
 * mode selection. The function for the third button and aswell as for
 * the encoder depends on the selected mode.
 * In the record mode for example, the encoder will be used for 
 * controlling the zoom and the function button will sart and stop
 * records. In any of the time-lapse config modes, the encoder will
 * select the value and the function button will start and stop
 * timelapse, the config will automatically send.
 */

#include <stdlib.h> // for malloc and free
void* operator new(size_t size) { return malloc(size); }
void operator delete(void* ptr) { if (ptr) free(ptr); } 



#include <ByteUtilities.h>
#include <OSCMessage.h>
#include <SLIP.h>
#include <SLIPSerialUsb.h>
#include <NewSoftSerial.h>
#include <Bounce.h>
#include <MemoryFree.h>

#include "EncoderClient.h"

EncoderClient *client;

void setup() {
  Serial.begin(9600);
      Serial.print("Mermory: ");
      Serial.println(freeMemory());
  Serial.println("Initializing client");
  client = new EncoderClient(4, 5, 8, 9, 10);
  client->setup();
}



void loop() {
  client->loop();
}

