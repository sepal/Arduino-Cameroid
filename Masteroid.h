
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
  virtual void init() = 0;

  /**
   * This function will be called in the loop.
   */
  virtual void update() = 0;

  /**
   * Will be triggered as soon as the Masteroid receives a OSC Messages via SLIP.
   * @param msg The message that was received.
   */
  virtual void eventOSC(OSC::Message *msg) = 0;

  /**
   * Sends an OSC Message via SLIP
   * @param msg The message to be send.
   */
  void sendOSC(OSC::Message *msg);
private:
  SLIPSerialUSB slip;
};

#endif