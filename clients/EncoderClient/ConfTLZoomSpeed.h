
#ifndef _CONFTLZOOMSPEED_H_
#define _CONFTLZOOMSPEED_H_

#include "WProgram.h"
#include "EncoderClientConfig.h"

class ConfTLZoomSpeed : public EncoderClientConfig
{
public:
  ConfTLZoomSpeed (NewSoftSerial *lcdSerial, SLIPSerialUSB *slip) : EncoderClientConfig(lcdSerial, slip)
  {
    msgValue.setAddress("/cameroid/timelapse/zoom/speed");
  }
  
  void init(InitConf *conf)
  {
    conf->encoderMode = InitConf::ENCODER_ABSOLUTE;
    conf->encMin = -12;
    conf->encMax = 12;
    conf->encPrec = 100;
    
    lcdClear();
    lcdSetLine(1);
    lcdSerial->print("TL Zm Speed");
    lcdSetLine(2);
    lcdSerial->print("Val:");
  }
  
  void loop() 
  {
  }
  
  void encoderChange(long val) {
    value = val;
    String s = String(value);
    lcdSetLine(2);
    lcdSetPos(16-s.length()-6);
    lcdSerial->print(s);
  }

  void buttonPressed(bool state) {
    if (state == HIGH) {
      msgValue.clear();
      msgValue.add(value);
      sendOSC(&msgValue);
    }
  }
private:
  int value;
  OSC::Message msgValue;
};

#endif
