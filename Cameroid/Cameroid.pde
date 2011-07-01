/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.1-0.x
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
 * Digital and automated camera control, currently for the Panasonic 
 * HMC151 only. 
 */

#include <EEPROM.h>
#include <ByteUtilities.h>
#include <OSCMessage.h>
#include <SLIP.h>
#include <SLIPSerialUsb.h>                              

#include "Cameroid.h"

Cameroid cam(7, 6, 5, 4);

void setup() {
  Serial.begin(9600);
  cam.setup();
}

void loop() {
  cam.loop();
}
