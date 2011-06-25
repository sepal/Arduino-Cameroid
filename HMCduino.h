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
 * Code for the HMCduino shield which basically consists of Xiroc X9C50P
 * Digital Potentiometer which controls the zoom and a MC14060 Quad 
 * Switch/Multiplexer. If you have a other potentiometer then the one with
 * 50k you might want to adjust the zoom speed range by defining the 4 macros
 * CAM_ZOOM_LOW_MIN, CAM_ZOOM_LOW_MAX, CAM_ZOOM_HI_MIN and CAM_ZOOM_HI_MAX 
 * before including this file. Although you could control the iris and the 
 * focus over a second (3.5mm 4-contact) TRS connector, I don't have plans
 * to expand the shield. One reason for that is the you would either need
 * at least two additional (expensive) Xirocs digipots, which each needs 
 * an additional pin, or I would have to use a 4 channel digipot. This 4 
 * channels digipots might be less expensive compared to the Xirocs, but 
 * use either SPI or I2C as communication protocol and, even worse, most
 * of them in a MSOP or TSSOP package.
 */

#ifndef _HMCDUINO_H_
#define _HMCDUINO_H_

// Define limits of the potentiometers.

// The limits zoom in area.
#define CAM_ZOOM_LOW_MIN 60
#define CAM_ZOOM_LOW_MAX 73

// The limits zoom out area.
#define CAM_ZOOM_HI_MIN 85
#define CAM_ZOOM_HI_MAX 98

#define CAM_ZOOM_OFF (CAM_ZOOM_LOW_MAX+(CAM_ZOOM_HI_MIN-CAM_ZOOM_LOW_MAX)/2)

#include "DigitPot.h"

/**
 * This class controls a Panasonic HMC151 using my HMCduino shield.
 */
class HMCControl
{
public:
  /**
   * Init all things. Be sure that the recording is turned of.
   */
  HMCControl(int recordPin, int potINCPin, int potUDPin, int potCSPin) : zoom(potINCPin, potUDPin, potCSPin)
  {
    pinRec = recordPin;
    pinMode(pinRec, OUTPUT);
    digitalWrite(pinRec, LOW);
    recState = false;
  }

  /**
   * Zoom in or out depending on the speed, which should be between -13 and 13.
   */
  void zooming(int speed) {
    if (speed > 0) {
      zoomIn(speed);
    } else if (speed<0) {
      zoomOut(abs(speed));
    } else {
      zoom.moveTo(CAM_ZOOM_OFF);
    }
  }

  /**
   * Zoom fully out and reset the wiper position.
   */
  void zoomReset(bool on) {
    if (on == true) {
      zoom.asyncMoveUp();
    } else {
      zoom.asyncMoveStop();
      zoom.resetCounter();
    }
  }

  void resetMode(bool on) {
    if (on == true)
    {
      zoom.asyncMoveUp();
    } else {
      zoom.asyncMoveStop();
      zoom.resetCounter();
    }
  }
  
  bool recording() {
    return recState;
  }

  /**
   * Turn record of or on.
   * Since the class don't actually knows if the camera is currently recording,
   * this function just makes sure that, you don't accidentally send to impulses.
   */
  void record(bool on) {
    if (on != recState) {
      recImpulse();
    }
  }

  void update() {
    zoom.update();
  }
protected:
  /**
   * Zoom in with the given speed.
   *
   * @param speed The speed of the zoom, should between 0 and 12;
   */
  void zoomIn(int speed) {
    speed = constrain(speed, 0, 12);
    zoom.moveTo(CAM_ZOOM_LOW_MAX-speed);
  }

  /**
   * Zoom out with the given speed.
   *
   * @param speed The speed of the zoom, should between 0 and 12;
   */
  void zoomOut(int speed) {
    speed = constrain(speed, 0, 12);
    Serial.println(CAM_ZOOM_HI_MIN-speed);
    zoom.moveTo(CAM_ZOOM_HI_MIN+speed);
  }

  int pinRec;
  int recState;
  DigiPot zoom;

  void recImpulse() {
    digitalWrite(pinRec, HIGH);
    delay(100);
    digitalWrite(pinRec, LOW);
    recState = !recState;
  }

};


#endif
