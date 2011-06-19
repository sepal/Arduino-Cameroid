/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.1-1.0-beta
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
 * Contains control class for Xiroc X9CXXX digital potentiometer.
 */

#ifndef _DIGIPOT_H_
#define _DIGIPOT_H_

/**
 * Controls a Xiroc X9CXXX digital potentiometer.
 *
 * @description
 * The Xicor X9C digital potentiometer series, are simple and low power CMOS chips.
 * They can be used instead of analog potentiometer to control various things.
 * There are versions with 10k, 20k, 50k and 100kOhms and they all come with memory
 * for the wiper position.
 * The down side of this chips are, that they don't give feedback. This class keeps 
 * track of the current position with an internal counter.
 */
class DigiPot
{
public:
  /**
   * Init the Digital Potentiometer.
   */
  DigiPot(int potINCPin, int potUDPin, int potCSPin) {
    pinINC = potINCPin;
    pinUD = potUDPin;
    pinCS = potCSPin;

    pinMode(pinCS, OUTPUT);
    // Imidiatly select the chip, so it don't save an unkown value.
    digitalWrite(pinCS, LOW);

    //Initialize the rest of the pins and set them to there start value.
    pinMode(pinINC, OUTPUT);
    pinMode(pinUD, OUTPUT);
    digitalWrite(pinINC, HIGH);
    digitalWrite(pinUD, LOW);

    wiperPos = 0;
    asyncMove = false;
    asyncDir = 0;
  }

  /**
   * Immediately move the Wiper to the given position.
   * 
   * @param pos The position (between 0 and 99) to which the wiper should be moved.
   */
  void moveTo(int pos) {
    if (asyncMove == false) {
      if (wiperPos < pos) {
        moveUpTo(pos);
      } else if (wiperPos > 0) {
        moveDownTo(pos);
      }
    }
  }

  /**
   * Move the wiper up without to block the system. You'll need to call update().
   */
  void asyncMoveUp() {
    asyncDir = 1;
    asyncMove = true;
    digitalWrite(pinUD, HIGH);
  }


  /**
   * Move the wiper down without to block the system. You'll need to call update().
   */
  void asyncMoveDown() {
    asyncDir = -1;
    asyncMove = true;
    digitalWrite(pinUD, LOW);
  }

  /**
   * Stop the asynchronus movement.
   */
  void asyncMoveStop() {
    asyncMove = false;
  }

  /**
   * Will either increase or decrease the wiper position.
   */
  int update() {
    if (asyncMove) {
      if (wiperPos > 99) {
        moveTo(99);
        asyncMove = false;
      } else if (wiperPos < 0) {
        moveTo(0);
        asyncMove = false;
      } else {
        digitalWrite(pinINC, LOW);
        delayMicroseconds(10);
        digitalWrite(pinINC, HIGH);
        wiperPos += asyncDir;
      }
    }
    return wiperPos;
  }

  /**
   * Return the wiperPosition.
   */
  int getWiperPos() {
    return wiperPos;
  }

  /**
   * Set the counter 99.
   */
  int resetCounter() {
    wiperPos = 99;
  }
protected:
  int pinINC;
  int pinCS;
  int pinUD;

  bool asyncMove;
  int asyncDir;

  int wiperPos;

  void moveDownTo(int pos) {
    if (pos > 0) {
      digitalWrite(pinUD, LOW);
      delayMicroseconds(100);
      while(wiperPos > pos) {
        digitalWrite(pinINC, LOW);
        delayMicroseconds(10);
        digitalWrite(pinINC, HIGH);
        wiperPos--;
      }
    }
  }

  void moveUpTo(int pos) {
    if (pos < 99) {
      digitalWrite(pinUD, HIGH);
      delayMicroseconds(100);
      while(wiperPos > pos) {
        digitalWrite(pinINC, LOW);
        delayMicroseconds(10);
        digitalWrite(pinINC, HIGH);
        wiperPos++;
      }
    }
  }
};

#endif