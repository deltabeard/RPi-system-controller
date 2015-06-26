# RPi-system-controller
An Arduino program to control the power of the Raspberry Pi and add features to its GPIO input
This program is made for my portable game console project. Incorporating an LCD connected to the composite port of the Raspberry Pi A+, two small speakers, buttons of a SNES controller, touch sensative L & R buttons at the back, 10000mAh battery from a battery pack, and a Powerboost 1000C.
An Arduino Pro Mini will be constantly powered on in powersave mode. An interrupt from the *Start* button will wake it up and check how long the button has been held down for. If the button has been held down for long enough, the Arduino will turn on the Powerboost 1000C which in turn will turn on the game console. Else, the Arduino will go back to powersave mode.
I have chosen to implement an Arduino to turn the device on and off, as using a switch to turn the device off does not gracefully shutdown the Raspberry Pi. Whearas the Arduino can check if the Raspberry Pi has properly shut down and then cut power to it.

## Features
- No need to have a power switch to turn the Raspberry Pi on and off. The Arduino will turn the Raspberry pi on when a button has been held down for a period of time. When the Raspberry Pi has been turned off using software, the Arduino will automatically cut power to the Raspberry Pi and the connected peripherals.
- Can use capacitive touch as inputs to the Raspberry Pi.
- Arduino stays in powersave mode to reduce power usage when device is turned off.
