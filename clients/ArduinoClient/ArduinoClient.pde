
/**
 * @file
 * @author  Sebastian Gilits <sep.gil@gmail.com>
 * @version 0.x-0.x
 *
 * @section LICENSE
 *
 * This file is part of Cameroid.
 * Cameroid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Cameroid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with Cameroid.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * A cameroid client using a digital potentiometer and 5 switches.
 * Three switches start/stop the recording, zoom resetting and timelapse
 * functions. The other are used to select varios modes, to set the
 * timelapse configuration. 
 * The potentiometer may be replaced by a rotary encoder some day.
 */
 
#include <ByteUtilities.h>
#include <OSCMessage.h>
#include <SLIP.h>
#include <SLIPSerialUsb.h>
#include <NewSoftSerial.h>


NewSoftSerial lcdSerial(2, 3);
SLIPSerialUSB slip;

void sendOSC(OSC::Message *msg) {
  int len = msg->length();
  unsigned char *packet = (unsigned char *) malloc(len);
  msg->getPacket(packet);
  slip.sendPacket(packet, len);
  free(packet);
}

void lcdSetLine(int line) {
  lcdSerial.print(254, BYTE);  // 254 bzw. 0xFE Befehl für Extended LCD Command
  lcdSerial.print(line==1 ? 128 : 192 , BYTE); // Beginn Position 1 in Zeile 1
}

void lcdClear() {
  lcdSerial.print(254, BYTE); 
  lcdSerial.print(1, BYTE); // Clear display
}

void lcdSetPos(int pos) {
  lcdSerial.print(0x80+pos, BYTE);
}
  

void setup() {
  Serial.begin(9600);
  lcdSerial.begin(9600);
  
  lcdSerial.print(124, BYTE);   // 124 bzw. 0x7C- Befehl für Displaybeleuchtung
  lcdSerial.print (157, BYTE);  // 128 (Disply off) - 157 (Display 100%) 
  
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
}

enum CONTROL_STATE
{
  STATE_MAIN,
  STATE_TL_REC_COUNTDOWN,
  STATE_TL_REC_DURATION,
  STATE_TL_ZOOM_COUNTDOWN,
  STATE_TL_ZOOM_CDOFFSET,
  STATE_TL_ZOOM_DURATION,
  STATE_TL_ZOOM_SPEED,
};

int lValPoti = 0;
int lValDig1 = 0;
int lValDig2 = 0;
int lValDig3 = 0;

CONTROL_STATE state = STATE_MAIN;

int rec = 0;
int resetZoom = 0;
int timelapse = 0;

void loop() {
  int valDig1 = digitalRead(4);
  int valDig2 = digitalRead(5);
  int valDig3 = digitalRead(6);
  
  
  if (state == STATE_MAIN) {
    int valPoti = map(analogRead(A0), 0, 1020, -12, 12);
    if (valPoti != lValPoti) {
      OSC::Message msg("/cameroid/zoom");
      msg.add(valPoti);
      sendOSC(&msg);

      lcdClear();
      lcdSetLine(1);
      lcdSerial.print ("-- Main --");
      lcdSetLine(2);
      lcdSerial.print ("Zoom: ");
      lcdSerial.print(valPoti);
    }
    lValPoti = valPoti;
  }

  if (valDig1 != lValDig1) {
    if (valDig1 == 1) {
      if (state == STATE_MAIN) {
        OSC::Message msg("/cameroid/record");
        rec = !rec;
        msg.add(rec);
        sendOSC(&msg);

        lcdClear();
        lcdSetLine(1);
        lcdSerial.print ("-- Main --");
        lcdSetLine(2);
        lcdSerial.print ("Recording: ");
        lcdSerial.print(rec);
      }
    }
    lValDig1 = valDig1;
  }
  if (valDig2 != lValDig2 ) {
    if (valDig2 == 1) {
      if (state == STATE_MAIN) {
        OSC::Message msg("/cameroid/zoomreset");
        resetZoom = !resetZoom;
        msg.add(resetZoom);
        sendOSC(&msg);
        
        lcdClear();
        lcdSetLine(1);
        lcdSerial.print ("-- Main --");
        lcdSetLine(2);
        lcdSerial.print ("Reset Zoom: ");
        lcdSerial.print(resetZoom);
      }
    }
    lValDig2 = valDig2;
  }
  if (valDig3 != lValDig3 ) {
    if (valDig3 == 1) {
      if (state == STATE_MAIN) {
        OSC::Message msg("/cameroid/timelapse");
        timelapse = !timelapse;
        msg.add(timelapse);
        sendOSC(&msg);
        
        lcdClear();
        lcdSetLine(1);
        lcdSerial.print ("-- Main --");
        lcdSetLine(2);
        lcdSerial.print ("Time-lapse: ");
        lcdSerial.print(timelapse);
      }
    }
    lValDig3 = valDig3;
  }
}

