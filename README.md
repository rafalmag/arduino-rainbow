# Arduino rainbow

Using neopixels sets the rainbow :)

Paradigms:
- easy to change the number of LEDs to be used
- do not use the blocking functions like delay / sleep
- fast math functions used 
- above allowed to use encoder without relying on interrupts
- power the case from AC mains (5V 5A 25W power supply used)
- be able to reprogram the Arduino without opening the case

## Build 1 review

It was very easy to start with a 47 leds star and then add 70 leds on the second day, by using the already prepared connectors.

While working on the breadboard propotype I do not encounter any issues with the encoder, however after builind the board the encoder was not very reliable - it was hard to generate a tick.
I tried to fix this by adding 10nF resistors betwen signal and ground, but it did not improve much. After using encoder library - the software debouncer logic helped a lot, but for second version the hardware debouncer would be recommended.

The 5V output from PSU is connected to VIN pin (although Arduino recommends min 7V) - the 5V output from Arduino is much lower, but it is only used for pulling up the digital pins for encoder (via resistors). Maybe this is causing the bouncing issue too.
The Arduino allows to connect both VIN and USB (then the higher voltage is used as an internal 5V  for powering the Arduino), so while powering from mains I can connect via usb to PC, flash new firware and test it while still USB connected.

Last issue with the design - I should move the encoder to a side, so it would be easier to press it with one hand (without holding the case with other).
