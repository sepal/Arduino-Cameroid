

#ifndef _ENCODERCLIENT_H_
#define _ENCODERCLIENT_H_

#include <MemoryFree.h>
#include "WProgram.h"
#include "EncoderClientConfig.h"
#include "EncoderClientConfOverview.h"
#include "RotaryEncoderRelative.h"
#include "RotaryEncoderAbsolute.h"


class EncoderClient {
public:
  EncoderClient(int pinEncA, int pinEncB, int pinFunction, int pinMenu, int pinZoomReset) 
  //  : btnFunction(pinFunction, 10), btnMenu(pinMenu, 10), btnZoomReset(pinZoomReset, 10)
  {
    pinMode(pinFunction, INPUT);
    pinMode(pinMenu, INPUT);
    pinMode(pinZoomReset, INPUT);
    encPinA = pinEncA;
    encPinB = pinEncB;
    
    this->pinButton[0] = pinFunction;
    this->pinButton[1] = pinMenu;
    this->pinButton[2] = pinZoomReset;
    
    for (int i; i<3; i++) {
      stateButton[i] = LOW;
    }
    
    lcdSerial = new NewSoftSerial(2, 3);
    lcdSerial->begin(9600);
    lcdSerial->print(124, BYTE);
    lcdSerial->print (157, BYTE);
    delay(1000);
    lcdSerial->print(254, BYTE);  // 254 bzw. 0xFE Befehl für Extended LCD Command
    lcdSerial->print(128, BYTE); // Beginn Position 1 in Zeile 1
    lcdSerial->print(254, BYTE);
    lcdSerial->print(1, BYTE);
    lcdSerial->print ("Initializing....");
    delay(1000);
    
    // Menu overview
    configs[0] = new ConfMenu(lcdSerial, &slip);
    // Live Zoom and recording contronl
    configs[1] = new ConfZoomRec(lcdSerial, &slip);
    // Configure Timelapse Recording countdown
    configs[2] = new ConfTLTime(lcdSerial, &slip, "/cameroid/timelapse/rec/countdown", "TL Rec Countdown", 2000);
    // Configure Timelapse Recording duration
    configs[3] = new ConfTLTime(lcdSerial, &slip, "/cameroid/timelapse/rec/duration", "TL Rec Duration", 1000);
    // Configure Timelapse zoom countdown
    /*configs[4] = new ConfTLTime(&lcdSerial, &slip, "/cameroid/timelapse/zm/countdown", "TL Zm Countdown", 500);
    // Configure Timelapse zoom countdown offset
    configs[5] = new ConfTLTime(&lcdSerial, &slip, "/cameroid/timelapse/zm/cdoffset", "TL Zm CDOffset", 1000);
    // Configure Timelapse zoom duration
    configs[6] = new ConfTLTime(&lcdSerial, &slip, "/cameroid/timelapse/zm/duration", "TL Zm Duration", 100);
    // Configure Timelapse zoom speed
    configs[7] = new ConfTLZoomSpeed(&lcdSerial, &slip);*/
  }
  
  void setup() {
    Serial.println("setup");
    changeMode(0);
  }
  
  void loop() {
    if (encoder->update()) {
      configs[currentConf]->encoderChange(encoder->getPosition());
    }
    
    for (int i=0; i<3; i++) {
      int cur = digitalRead(pinButton[i]);
      if (cur != stateButton[i]) {
        switch(i) {
          case 0:
            if (currentConf > 0) {
              configs[currentConf]->buttonPressed(cur == HIGH ? true : false);
            } else {
              if (cur==HIGH) {                
                int idx = configs[currentConf]->buttonMenu2Pressed();
                changeMode(idx);
              }
            }
            break;
          case 1:
            if (cur == HIGH) {
               changeMode(configs[currentConf]->buttonMenuPressed());
            }
            break;
          case 2:
            if (cur == HIGH) {
               configs[currentConf]->buttonZoomResetPressed();
            }
            break;
        }
        stateButton[i] = cur;
      }
    }
    configs[currentConf]->loop();
  }
  
private:
  int encPinA, encPinB;
  int pinButton[3];
  RotaryEncoder *encoder;
  int stateButton[3];
  /*Bounce btnFunction;
  Bounce btnMenu;
  Bounce btnZoomReset;*/
  
  NewSoftSerial *lcdSerial;
  SLIPSerialUSB slip;
  
  EncoderClientConfig *configs[8];
  int currentConf;

  void changeMode(int idx) {
    if (idx >= 0) {
      delay(100);
      currentConf = idx;
      InitConf conf;
      configs[currentConf]->init(&conf);
      delete encoder;
      Serial.println("Creating Encoder");
      switch (conf.encoderMode) {
        case InitConf::ENCODER_RELATIVE:
          encoder = new RelativeRotaryEncoder(encPinA, encPinB, conf.encMin, conf.encMax, conf.encPrec);
          break;
        case InitConf::ENCODER_ABSOLUTE:
          encoder = new RotaryEncoderAbsolute(encPinA, encPinB, conf.encMin, conf.encMax, conf.encPrec);
          break;
      }
      Serial.println(freeMemory());
    }
  }
 
};

#endif
