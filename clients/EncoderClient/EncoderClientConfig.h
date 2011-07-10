

#ifndef _ENCODERCLIENTCONFIG_H_
#define _ENCODERCLIENTCONFIG_H_

#include "WProgram.h"
#include "InitConf.h"
#include <NewSoftSerial.h>


class EncoderClientConfig
{
public:
  EncoderClientConfig(NewSoftSerial *lcdSerial, SLIPSerialUSB *slip) 
  {
    this->lcdSerial = lcdSerial;
    this->slip = slip;
  }
  
  virtual void init(InitConf *conf) {}
  virtual void loop() {}

  virtual void buttonPressed(bool state) {}
  virtual void encoderChange(long val) {}
  
  virtual int buttonMenu2Pressed() {
    return 0;
  }
  virtual int buttonMenuPressed() {
    return 0;
  }
  
  void buttonZoomResetPressed() {
    zoomReset = !zoomReset;
    OSC::Message msg("/cameroid/zoomreset");
    msg.add(zoomReset);
    sendOSC(&msg);
  }
protected:
  NewSoftSerial *lcdSerial;
  
  void sendOSC(OSC::Message *msg) {
    int len = msg->length();
    unsigned char *packet = (unsigned char *) malloc(len);
    msg->getPacket(packet);
    slip->sendPacket(packet, len);
    free(packet);
  }

  void lcdSetLine(int line) {
    lcdSerial->print(254, BYTE);  // 254 bzw. 0xFE Befehl f�r Extended LCD Command
    lcdSerial->print(line==1 ? 128 : 192 , BYTE); // Beginn Position 1 in Zeile 1
  }

  void lcdClear() {
    lcdSerial->print(254, BYTE); 
    lcdSerial->print(1, BYTE); // Clear display
  }

  void lcdSetPos(int pos) {
    lcdSerial->print(254, BYTE);
    lcdSerial->print(0x80+pos, BYTE);
  }
private:
  SLIPSerialUSB *slip;
  int menuState;
  int zoomReset;
};

#endif
