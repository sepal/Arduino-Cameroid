--------------------------------------------------------------------------------
                                    Cameroid
                   Arduino camera control and automatization
--------------------------------------------------------------------------------


About
-----

This Arduino sketch controls a Panasonic Hmc151 through the TRS jack. Currently
it only will work for the zoom and to start & stop records. Since the Cam 
itself lacks of a time-lapse function, I added a feature which allows you to
automatically record short clips, from witch you can extract the images to 
create time-lapse clips. You can watch short example here:
http://www.youtube.com/watch?v=G13pcyFE5Dw

All parameter are configurable through Serial SLIP OSC, which means, you can 
connect the arduino to your computer and cantrol the cameroid through my 
processing test client, or you could connect a second arduino to the cameroid 
arduino and send Serial SLIP OSC messages to control it. Therefore you could
even control it wirelessly using xbee.
I'll try to release more client examples in the future.


Features
--------

Currently (for the Panasonic HMC151 only)
* Control the Zoom
* Start or stop records.
* Start and stop configurable time-lapse records.
* Save time-lapse configuration to the EEPROM.

Planned:
* Dolly control using this DIY Slider: 
    http://www.pocketslider.de/DIY_slider_en.html
* Focus and Iris control throught the second TRS jack.
* Learn movement functionality


Requirements
-------------

Hardware:
* Arduino (code is currently optimized for UNO)
* An Xiroc X9C503 50k digital Potentiometer
* An MC14060 Quad digital switch/multiplexer

Software:
* My SLIP OSC class ()

Contact/Support
---------------

Use this library at your one risk, I can't and won't give any garanties that
this programm will work with your camera. Use it at your own risk.

If you have any questions about this project contact me via github, if you find
bugs or want to request features use the issue queue of the github repository.

Visit my portfolio at: http://sepgil.bplaced.net/