/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.1-0.1
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
 * Control a HMC using the HMCduino board. Future versions will also 
 * support dolly control.
 */


#ifndef _CAMEROID_H_
#define _CAMEROID_H_

#include <EEPROM.h>
#include <ByteUtilities.h>
#include "Masteroid.h"
#include "HMCduino.h"
#include "SoftTimer.h"

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
    
    // Load the last saved timelapse config from the EEPROM.
    timeLapseLoad();
  }

  void update()
  {
    hmc.update();
    if (timelapse) {
      if (trRecCountdown.ready()) {
        hmc.record(true);
        trRecDuration.start(tlRecDuration);
      }
      if (trRecDuration.ready()) {
        hmc.record(false);
        trRecDuration.stop();
      }
      if (trZoomCountDown.ready()) {
        hmc.zooming(tlZoomSpeed);
        trZoomDuration.start(tlZoomDuration, tlZommCDOffset);
      }
      if (trZoomDuration.ready()) {
        hmc.zooming(0);
        trZoomDuration.stop();
      }
    }
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
      } else if (address.startsWith("/cameroid/timelapse")) {
        timeLapseOSC(msg);
      }
    }
  }
private:
  
  HMCControl hmc;

  bool timelapse;
  // Time-lapse paramters

  
  // Time for the next shot.
  float tlRecCountdown;
  // How long should one record take.
  float tlRecDuration; 
  // Where should the zoom start to move. Camera will have to reset
  // the digipot to determain the (almost) exact position.
  float tlZoomInit;
  // The speed of the zoom.
  int tlZoomSpeed;
  // Countdown for the zoom to move.
  float tlZoomCountDown;
  // Timr offset.
  float tlZommCDOffset;
  // How long should be zoomed.
  float tlZoomDuration;
  
  SoftTimer trRecCountdown;
  SoftTimer trRecDuration;
  SoftTimer trZoomCountDown;
  SoftTimer trZoomDuration;
  
  /**
   * Save the time-lapse config into the EEPROM.
   */
  void timeLapseSave() {
    byte mem[5][3];
    ByteUtilities::float2Byte(tlRecCountdown, mem[0]);
    ByteUtilities::float2Byte(tlRecDuration, mem[1]);
    ByteUtilities::float2Byte(tlZoomCountDown, mem[2]);
    ByteUtilities::float2Byte(tlZommCDOffset, mem[3]);
    ByteUtilities::float2Byte(tlZoomDuration, mem[43]);
    ByteUtilities::float2Byte(tlZoomSpeed, mem[5]);
    
    int idx = 0;
    for (int i=0; i<6; i++) {
      for (int j=0; j<4; j++) {
        EEPROM.write(idx, mem[i][j]);
        idx++;
      }
    }
  }
  
  /**
   * Load the timelapse config form the EEPROM.
   */
  void timeLapseLoad() {
    byte mem[5][3];
    
    int idx = 0;
    for (int i=0; i<6; i++) {
      for (int j=0; j<4; j++) {
        mem[i][j] = EEPROM.read(idx);
        idx++;
      }
    }
    
    tlRecCountdown = ByteUtilities::byte2Float(mem[0]);
    tlRecDuration = ByteUtilities::byte2Float(mem[1]);
    tlZoomCountDown = ByteUtilities::byte2Float(mem[2]);
    tlZommCDOffset = ByteUtilities::byte2Float(mem[3]);
    tlZoomDuration = ByteUtilities::byte2Float(mem[4]);
    tlZoomSpeed = ByteUtilities::byte2Float(mem[5]);
  }
  
  /**
   * Handle the OSC messages for time-lapse.
   */
  void timeLapseOSC(OSC::Message *msg) {
    String address = msg->getAddress();
    String typetag = msg->getTypeTag();
    if (address.equals("/cameroid/timelapse/config") && typetag.equals("fffffi")) {
      Serial.println("Recieved timelapse config.");
      tlRecCountdown = msg->getFloat(0);
      tlRecDuration = msg->getFloat(1);
      tlZoomCountDown = msg->getFloat(2);
      tlZommCDOffset = msg->getFloat(3);
      tlZoomDuration = msg->getFloat(4);
      tlZoomSpeed = msg->getInt(5);
    }
    if (address.equals("/cameroid/timelapse/start") && typetag.equals("i")) {
      if (msg->getInt(0) == 1) {
        Serial.println("Starting timeplapse.");
        timelapse = true;
        trRecCountdown.start(tlRecCountdown);
        trZoomCountDown.start(tlZoomCountDown);
      } else {
        Serial.println("Stoping timeplapse.");
        hmc.record(false);
        trRecCountdown.stop();
        trZoomCountDown.stop();
        trRecDuration.stop();
        trZoomDuration.stop();
        timelapse = false;
      }
    } else if (address.equals("/cameroid/timelapse/save")) {
      timeLapseSave();
    }
  }
};

#endif
