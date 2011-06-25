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
 */

#ifndef _SOFTTIMER_H_
#define _SOFTTIMER_H_

#include "WProgram.h"

class SoftTimer
{
public:
  SoftTimer() {
    started = false;
    last = 0;
    time = 0;
  }
  void start(float seconds) {
    time = seconds*1000;
    last = millis();
    started = true;
  }
  
  void stop() {
    started = false;
  }
  
  bool ready() {
    if (started) {
      if (millis()-last > time) {
        last = millis();
        return true;
      }
    }
    return false;
  }
  
private:
  long last;
  long time;
  bool started;
};

#endif
