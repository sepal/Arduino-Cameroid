

#ifndef _RELATIVEROTARYENCODER_H_
#define _RELATIVEROTARYENCODER_H_

#include "WProgram.h"
#include "RotaryEncoder.h"

class RelativeRotaryEncoder : public RotaryEncoder {
public:
  /**
   * Creates a new encoder class.
   * @param pinA First encoder line.
   * @param pinB Second encoder line
   * @param min The minimum value.
   * @param max The maximum value.
   * @param precision How many encoder steps until the rel. position changes
   */
  RelativeRotaryEncoder(int pinA, int pinB, long min, long max, int precision) : RotaryEncoder(pinA, pinB, min, max, precision)
  {
    realPos = 0;
    pos = (max+min)/2;
  }

  boolean update() {
    boolean ret = false;
    n = digitalRead(outA);
    if ((outALast == LOW) && (n == HIGH)) {
      realPos++;
      if (realPos > precision) {
        if (digitalRead(outB) == LOW) {
          pos--;
          if (pos < min) {
            pos = min;
          }
        } else {
          pos++;
          if (pos > max) {
            pos = max;
          }
        }
        realPos = 0;
        ret = true;
      }
    }
    outALast = n;
    return ret;
  }
private:
  long realPos;
};

#endif
