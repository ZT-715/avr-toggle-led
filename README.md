avr-toggle-led
=================

### Overview

Makes the 'L' led (port b5) on arduino nano bord toggle on and off with signal change on port b4 (D12 pin on arduino).

![Alt text](https://media.giphy.com/media/KBlmR3WVKoy9XZIq1Q/giphy-downsized-large.gif)

On the board, the button (when pressed) grounds pin D12 (witch is internally pulled up) causing an abrupt signal change to trigger the interrupt.

The change in state of the led is then written to the eeprom, saving its state even after power offs and resets.


### Setup

(CMake file adapted from [CMake-avr-example](https://github.com/patrick--/CMake-avr-example.git), modify the definitions in `CMakeLists.txt` as needed)

Uses the ATMega328p running at 16Mhz and is programmed through arduino ISP:

* Microcontroller : `set(MCU  atmega328p)`
* Clock speed : `set(F_CPU 16000000)`
* Baud rate: `set(BAUD 57600)`
* Programmer: `set(PROG_TYPE arduino)`
* [Arduino port](https://www.mathworks.com/help/supportpkg/arduinoio/ug/find-arduino-port-on-windows-mac-and-linux.html): `set(PORT /dev/ttyUSB0)`


#### Cloning, Compiling and flashing
Instructions below assume you have successfully installed the [AVR toolchain](https://www.nongnu.org/avr-libc/user-manual/overview.html) and [CMake](http://www.cmake.org/):
```sh
git clone 
cd avr-toggle-led
mkdir build
cd build
cmake ..
make flash
```
