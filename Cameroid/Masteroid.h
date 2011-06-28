/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.1-0.x
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
 * along with Cameroid.  If not, see <http://www.gnu.org/licenses/>. at
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef _MASTEROID_H_
#define _MASTEROID_H_


#include "SLIPSerialUsb.h"
#include "OSCMessage.h"


/**
 * Abstract Masteroid class that allows you to send and receive OSC Messages.
 * Only override the virtual functions.
 */
class Masteroid
{
public:
  /**
   * Creates a new Masteroid.
   */
  Masteroid();

  /**
   * Initializes all variables.
   */
  void setup();

  /**
   * Updates everything.
   */
  void loop();
protected:
  /**
   * This function will be called on setup.
   */
  virtual void init() {};

  /**
   * This function will be called in the loop.
   */
  virtual void update() {};

  /**
   * Will be triggered as soon as the Masteroid receives a OSC Messages via SLIP.
   * @param msg The message that was received.
   */
  virtual void eventOSC(OSC::Message *msg) {};

  /**
   * Sends an OSC Message via SLIP
   * @param msg The message to be send.
   */
  void sendOSC(OSC::Message *msg);
private:
  SLIPSerialUSB slip;
};

#endif
