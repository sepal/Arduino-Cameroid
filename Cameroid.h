/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.1-0.1
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 * Control a HMC using the HMCduino board. Future versions will also 
 * support dolly control.
 */

#include "Masteroid.h"
#include "HMCduino.h"

#ifndef _CAMEROID_H_
#define _CAMEROID_H_

/**
 * Masteroid for a Panasonic HMC151 camera.
 * Can control the zoom, as well as start and stop recordings. 
 * Also comes with a time-lapse function, which records short clips(since
 * the cam can't take only photos). All paramters are configurable over OSC.
 * The timelapse function will currently use the millis function to determain
 * the counddown time, but may be replaced by the atmega chips internal hardware 
 * timers some day.
 * Another future feature would be the learn function which will allow you to 
 * records certain moves and play them back.
 */
class Cameroid : public Masteroid
{
public:
  Cameroid(int recordPin, int potINCPin, int potUDPin, int potCSPin) : hmc(recordPin, potINCPin, potUDPin, potCSPin)
  {}
protected:
  void init()
  {
    // Send init message.
    OSC::Message msg("/sys/init");
    // Main container
    msg.add("cameroid");
    // Send 1 if cameroid was initialized well.
    // Will be replaced by TRUE/FALSE types.
    msg.add(1);
    // Additional display message for the user.
    msg.add("Cameroid initialized.");
    this->sendOSC(&msg);
  }

  void update()
  {
    
  }

  void eventOSC(OSC::Message *msg)
  {
    String address = msg->getAddress();
    String typetag = msg->getTypeTag();
    if (address.startsWith("/cameroid"))
    {
      if (address.equals("/cameroid/zoom") && typetag.equals("i")) {
        int speed = msg->getInt(0); 
        hmc.zooming(speed);
      } else if (address.equals("/cameroid/zoomreset") && typetag.equals("i"))
      {
        hmc.zoomReset((msg->getInt(0)) ? true : false);
      } else if (address.equals("/cameroid/record") && typetag.equals("i"))
      {
        hmc.record((msg->getInt(0)) ? true : false);
      }
    }
  }
private:
  HMCControl hmc;

  bool timelapse;
  // Time-lapse paramters

  // If set(>1), the time-laspe will function for the given duration and all 
  // other timer paramters may be set automatically(look for the other notes).
  long tlDuration; 
  // Time for the next shot.
  long tlRecCountdown;
  // How long should one record take.
  long tlRecDuration; 
  // Where should the zoom start to move. Camera will have to reset
  // the digipot to determain the (almost) exact position.
  int tlZoomInit;
  // If set(>-1) together with tlDuration and tlZoomSpeed, the programm will
  // automatically also set tlZoomCountDown.
  int tlZoomEnd;
  // The speed of the zoom.
  long tlZoomSpeed;
  // Countdown for the zoom to move.
  long tlZoomCountDown;
};

#endif
