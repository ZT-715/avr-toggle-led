avr-toggle-led
=================

## Overview

Makes the 'L' LED (port B5) on Arduino Nano board toggle on and off with signal change (HIGH to LOW) on port D5 or any serial input (UART) on USB, then shares current state ('on' or 'off') as serial output.

![Alt text](https://media.giphy.com/media/agZbFIbxs24hRY06JE/giphy-downsized-large.gif)

On the board, the button (when pressed) grounds pin D5 (witch is internally pulled up) causing an abrupt signal change to trigger the pin's interrupt that if LOW raises a flag to toggle the LED and deactivates itself for a period.

The "toggle-led" flag raised will be detected on the main loop (pulling), the  state of the LED is then toggled and written to the EEPROM, saving it even after power-offs and resets.

The 16bit timer/counter is used to define strict period control on frequency of each interrupt, enabling an interrupt reactivation only after at least 500ms of its last handling, without need of delays in the main loop.

## Setup

(CMake file adapted from [CMake-avr-example](https://github.com/patrick--/CMake-avr-example.git), modify the definitions in `CMakeLists.txt` as needed)

Uses the ATMega328p running at 16Mhz and is programmed through arduino ISP:

* Microcontroller : `set(MCU  atmega328p)`
* Clock speed : `set(F_CPU 16000000)`
* Baud rate: `set(BAUD 57600)`
* Programmer: `set(PROG_TYPE arduino)`
* [Arduino port](https://www.mathworks.com/help/supportpkg/arduinoio/ug/find-arduino-port-on-windows-mac-and-linux.html): `set(PORT /dev/ttyUSB0)`

### Cloning, Compiling and flashing

Instructions below assume you have successfully installed the [AVR toolchain](https://www.nongnu.org/avr-libc/user-manual/overview.html) and [CMake](http://www.cmake.org/):

```sh
git clone 
cd avr-toggle-led
mkdir build
cd build
cmake ..
make flash
```

### Console

Uses 8 bit of data, 1 stop bit and no parity bit and may be used by any serial console program just by setting the arduino port and the baud rate, ex.:

```shell
screen /dev/ttyUSB0 57600
```

or:

```shell
cu -l /dev/ttyUSB0 -s 57600
```

or using CMAKE:

```shell
make serial
```
