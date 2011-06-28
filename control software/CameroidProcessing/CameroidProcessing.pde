import java.util.regex.Pattern;
import java.util.regex.Matcher;

import java.nio.ByteBuffer;
import processing.serial.*;
import oscP5.*;
import controlP5.*;

int SERIAL_PORT = 0;
Serial device;

ControlP5 controlP5;
Slider zoom;

Textfield tlRecCountdown;
Textfield tlRecDuration;
Textfield tlZoomDuration;
Textfield tlZoomSpeed;
Textfield tlZoomCountDown;

Button tlSend;
Button tlStart;


void setup() {
  println(Serial.list());
  device = new Serial(this, Serial.list()[SERIAL_PORT], 115200);
  
  size(400,400);
  controlP5 = new ControlP5(this);
  
  // Create GUI
  zoom = (Slider)controlP5.addSlider("slider",-13,13,1,20,20,20,256);
  zoom.setNumberOfTickMarks(27);
  zoom.setSliderMode(Slider.FLEXIBLE);
  
  Button rec = controlP5.addButton("Record",1,100,130,80,19);
  Button rst = controlP5.addButton("ResetZoom",1,100,150,80,19);
  rec.setSwitch(true);
  rst.setSwitch(true);
  
  //controlP5.addButton("Zoom Test",255,100,150,80,19);
  
  // Timelapse config GUI
  tlRecCountdown = controlP5.addTextfield("Rec Timer",200,10,49,20);
  tlRecDuration = controlP5.addTextfield("Rec Time",250,10,50,20);
  tlZoomCountDown = controlP5.addTextfield("Zm timer",200,45,50,20);
  tlZoomDuration = controlP5.addTextfield("Zm Time",250,45,50,20);
  tlZoomSpeed = controlP5.addTextfield("Zm Speed",300,45,50,20);
  
  tlSend = controlP5.addButton("SendCnf",0,200,90,49,20);
  tlStart = controlP5.addButton("StartTL",1,250,90,49,20);
  tlStart.setSwitch(true);
  
}
  

void draw() {
  background(100);
  
  fill(100);
  rect(0,0,width,100);
  
  fill(50);
  rect(0,300,width,70);
  
  fill(30);
  rect(0,370,width,30);
}

void slider(float value) {
  println("Slider changed");
  OscMessage zoom = new OscMessage("/cameroid/zoom");
  zoom.add((int)value);
  sendSLIP(zoom.getBytes());
}

int recording = 0;

void Record(float value) {
  println("Record");
  OscMessage rec = new OscMessage("/cameroid/record");
  recording = (recording == 1) ? 0 : 1;
  rec.add(recording);
  sendSLIP(rec.getBytes());
}

int reseting = 0;
void ResetZoom(float value) {
  println("Reset zoom");
  OscMessage rzoom = new OscMessage("/cameroid/zoomreset");
  reseting = (reseting == 1) ? 0 : 1;
  rzoom.add(reseting);
  sendSLIP(rzoom.getBytes());
}


void SendCnf(float value) {
  sendTimeLapseConfig();
}

int timeLapse = 0;
void StartTL(float value) {
  timeLapse = (timeLapse == 1) ? 0 : 1;
  OscMessage tlStart = new OscMessage("/cameroid/timelapse/start");
  tlStart.add(timeLapse);
  sendSLIP(tlStart.getBytes());
  
}

void sendTimeLapseConfig() {
  OscMessage tlConfig = new OscMessage("/cameroid/timelapse/config");
  
  float recCd = Float.valueOf(tlRecCountdown.getText()).floatValue();
  float recDr = Float.valueOf(tlRecDuration.getText()).floatValue();
  float zmCD = Float.valueOf(tlZoomCountDown.getText()).floatValue();
  float zmDr = Float.valueOf(tlZoomDuration.getText()).floatValue();
  int zmSp = Integer.valueOf(tlZoomSpeed.getText()).intValue();


  tlConfig.add(recCd);
  tlConfig.add(recDr);
  tlConfig.add(zmCD);
  tlConfig.add(zmDr);
  tlConfig.add(zmSp);
  
  sendSLIP(tlConfig.getBytes());
  
}

// Serial & OSC Stuff
final int END = 0300;
final int ESC = 0333;
final int ESC_END = 0334;
final int ESC_ESC = 0335;

int count = 0;

void sendSLIP(byte[] packet) {
  int len = packet.length;
  device.write(END);
  for (int i=0; i<packet.length; i++) {
    switch (packet[i]) {
      case (byte)END:
        device.write(ESC);
        device.write(ESC_END);
        break;
      case (byte)ESC:
        device.write(ESC);
        device.write(ESC_END);
        break;
      default:
        device.write(packet[i]);
    }
  }
  device.write(END);
}

void serialEvent(Serial p) {
  //int buffer = device.read();
  //println(hex(buffer) + " " + char(buffer));
  while (device.available() > 0) {
    String inBuffer = device.readString();   
    if (inBuffer != null) {
      print(inBuffer);
    }
  }
}

