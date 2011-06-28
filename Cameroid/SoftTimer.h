/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.1-1.0
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
 * along with Cameroid.  If not, see <http://www.gnu.org/licenses/>. at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A simple timer that uses the millis function to determine if a 
 * certain time has passed.
 */

#ifndef _SOFTTIMER_H_
#define _SOFTTIMER_H_

#include "WProgram.h"

/**
 * This class will allow you to determine if a certain time has passed.
 * It does that using the arduino function millis, so I don't know how
 * accurate it is, but it's certain that will not work for peroids 
 * longer than approximately 50 days, since the number will then 
 * overflow.
 */
class SoftTimer
{
public:
  /**
   * create and initialize the timer.
   */
  SoftTimer() {
    started = false;
    last = 0;
    time = 0;
    offset = 0;
  }
  
  /**
   * Start the timer.
   * 
   * @param seconds Seocnds until the timer triggers use floating point
   *   numbers to for miliseconds
   * 
   */
  void start(float seconds) {
    time = seconds*1000;
    last = millis();
    started = true;
  }
  /**
   * Start the timer with a delay.
   * 
   * @param seconds Seocnds until the timer triggers use floating point
   *   numbers to for miliseconds
   *
   * @param startDelay delays the first countdown for the given time in 
   *   seocnds, usefull for timers which trigger altenate to others.
   * 
   */
  void start(float seconds, float startDelay) {
    offset = startDelay*1000;
    start(seconds);
  }
  
  /**
   * Stop the timer.
   */
  void stop() {
    started = false;
  }
  
  /**
   * Updates the timer and notifies if it has triggered.
   * 
   * @return True if the timer has triggered.
   */
  bool ready() {
    if (started) {
      if (millis()-last > time+offset) {
        last = millis();
        offset = 0;
        return true;
      }
    }
    return false;
  }
  
private:
  long last;
  long time;
  bool started;
  int offset;
};

#endif
