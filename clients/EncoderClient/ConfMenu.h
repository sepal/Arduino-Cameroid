
#ifndef _CONFMENU_H_
#define _CONFMENU_H_

#include "WProgram.h"
#include "EncoderClientConfig.h"

class ConfMenu : public EncoderClientConfig
{
public:
  ConfMenu (NewSoftSerial *lcdSerial, SLIPSerialUSB *slip) : EncoderClientConfig(lcdSerial, slip)
  {
    current = 0;
  }
  
  void init(InitConf *conf)
  {
    Serial.println("Init ConfMenu");
    conf->encoderMode = InitConf::ENCODER_ABSOLUTE;
    conf->encMin = 0;
    conf->encMax = 2;
    conf->encPrec = 100;
    
    // Mode names
    //                1234567890123456
    menuEntries[0] = "Zoom/Rec        ";
    menuEntries[1] = "TL Rec Countdown";
    menuEntries[2] = "TL Rec Duration ";
    /*menuEntries[3] = "TL Zm  Countdown";
    menuEntries[4] = "TL Zm  CDOffset ";
    menuEntries[5] = "TL Zm  Duration ";
    menuEntries[6] = "TL Zm  Speed    ";
    Serial.println("Initialized ConfMenu");*/
    lcdClear();
    lcdSetLine(1);
    lcdSerial->print("Select mode:");
    encoderChange(0);
  }
  
  void loop() 
  {
  }

  void buttonPressed(bool state) {}
  
  void encoderChange(long val) {
    delay(100);
    lcdSetLine(2);
    lcdSerial->print(menuEntries[val]);
    current = val;   
  }
  
  int buttonMenu2Pressed() {
    Serial.print("Menu selected:");
    Serial.println(current+1);
    return current+1;
  }
  
  int buttonMenuPressed() {
    Serial.println("Menu pressed");
    return -1;
  }
private:
  String menuEntries[7];
  int current;
};

#endif
