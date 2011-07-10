
#ifndef _CONFZOOMREC_H_
#define _CONFZOOMREC_H_

#include "WProgram.h"
#include "EncoderClientConfig.h"

class ConfZoomRec : public EncoderClientConfig
{
public:
  ConfZoomRec (NewSoftSerial *lcdSerial, SLIPSerialUSB *slip) : EncoderClientConfig(lcdSerial, slip)
  {
    msgZoom.setAddress("/cameroid/zoom");
    msgRec.setAddress("/cameroid/record");
  }
  
  void init(InitConf *conf)
  {
    conf->encoderMode = InitConf::ENCODER_RELATIVE;
    conf->encMin = -12;
    conf->encMax = 12;
    conf->encPrec = 75;
    
    lcdClear();
    lcdSetLine(1);
    lcdSerial->print("Zoom/Rec");
    lcdSetLine(2);
    lcdSerial->print("Zoom:    : Rec:0");
  }
  
  void loop() 
  {
  }
  
  void encoderChange(long val) {
    msgZoom.clear();
    msgZoom.add((int)val);
    sendOSC(&msgZoom);
    
    String s = String(val);
    lcdSetLine(2);
    lcdSerial->print("Zoom:");
    for (int i=0; i<3-s.length();i++) {
      lcdSerial->print(" ");
    }
    lcdSerial->print(s);
  }

  void buttonPressed(bool state) {
    if (state == HIGH) {
      recording = !recording;
      
      msgRec.clear();
      msgRec.add(recording);
      //sendOSC(&msgRec);
      
      lcdSerial->print(254, BYTE);
      lcdSerial->print(199, BYTE);
      lcdSerial->print(String(": Rec:").concat(recording));
    }
  }
  
private:
  OSC::Message msgZoom;
  OSC::Message msgRec;
  
  int recording;
};

#endif
