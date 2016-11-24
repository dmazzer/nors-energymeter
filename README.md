# NORS Energy Metering Sensor

## Introduction

This sensor is part of the [NORS project](https://github.com/dmazzer/nors).
The sensor measures voltage and current from 2 phases. 
The NORS project already include this sensor as a git submodule, so this repository may be used alone only for development purposes.

The application is composed by two different systems:

### Arduino

The arduino code is in folder arduino-mega2560. This code that acquires voltage and current from AC lines.
To build the code you can use [inotool](http://inotool.org/), a command line tool to build arduino projects and to upload them to the hardware. With inotool you can build and upload the code from a RaspberryPi.
The energy metering is based on EmonLib library ([project page](https://openenergymonitor.org/emon/), [github](https://github.com/openenergymonitor/EmonLib)).

### Python

The python code is in root folder of the project and it is split in two files, sensor_ports.py and sensor_energymeter.py. The first is a helper to manage serial ports and the second is where the sensor becames a NORS sensor.

