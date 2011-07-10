

#ifndef _ROTARYENCODER_H_
#define _ROTARYENCODER_H_

#include "WProgram.h"

class RotaryEncoder {
public:
  /**
   * Creates a new encoder class.
   * @param pinA First encoder line.
   * @param pinB Second encoder line
   */
  RotaryEncoder(int pinA, int pinB, long min, long max, int precision) {
    outA = pinA;
    outB = pinB;
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    this->min = min;
    this->max = max;
    pos = 0;
    outALast = LOW;
    n = LOW;
    this->precision = precision;
  }

  /**
   * Returns the position.
   */
  long getPosition() {
    return pos;
  }

  /**
   * Sets the position
   */
  void setPosition(long position) {
    pos = position;
  }

  /**
   * The minimum limit for the relative position.
   */
  void setMin(int min) {
    this->min = min;
  }

  /**
   * The maximum limit for the relative position.
   */
  void setMax(int max) {
    this->max = max;
  }

  /**
   * Update the position.
   */
  virtual boolean update() {};
  
protected:
  int outA, outB;
  int outALast, n;
  long pos;
  long min, max;
  int precision;
};

#endif
