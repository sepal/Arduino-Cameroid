
#ifndef _CONFTLTIME_H_
#define _CONFTLTIME_H_

#include "WProgram.h"
#include "EncoderClientConfig.h"
#include <stdlib.h>

class ConfTLTime : public EncoderClientConfig
{
public:
  ConfTLTime (NewSoftSerial *lcdSerial, SLIPSerialUSB *slip, String oscAddress, String title, int min) : EncoderClientConfig(lcdSerial, slip)
  {
    this->title = title;
    encMin = min;
    msgValue.setAddress(oscAddress);
  }
  
  void init(InitConf *conf)
  {
    conf->encoderMode = InitConf::ENCODER_ABSOLUTE;
    conf->encMin = encMin;
    conf->encMax = 604800;
    conf->encPrec = 25;
    
    lcdClear();
    lcdSetLine(1);
    lcdSerial->print(title);
    lcdSetLine(2);
    lcdSerial->print("Val:");
  }
  
  void loop() 
  {
  }
  
  void encoderChange(long val) {
    value = val/1000;
    
    char s[10];
    dtostrf(val, 1, 3, s);
    
    lcdSetLine(2);
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
  String title;
  float value;
  int encMin;
  OSC::Message msgValue;
};

#endif
