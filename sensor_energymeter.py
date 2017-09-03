#!/usr/bin/env python2

"""
sensor_power.py: Current and Voltage sensor driver

This sensor driver collects current and voltage measurements from external arduino board.
"""

__author__ = "Daniel Mazzer"
__copyright__ = "Copyright 2016, NORS project"
__credits__ = ""
__license__ = "MIT"
__maintainer__ = "Daniel Mazzer"
__email__ = "dmazzer@gmail.com"

import signal
import serial

from genericsensor.genericsensor import Nors_GenericSensor
from grovepi import *
from norsutils.logmsgs.logger import Logger
from serial_ports import serial_ports

logger = Logger('debug')
logger.log("SENSOR - Dual phase power meter", 'info')

class RealSensor(Nors_GenericSensor):
    def __init__(self):

        self.sensor_name = 'PWRMTR'
        self.message = {}

        self.no_data_timeout_reset = 5;
        self.no_data_timeout = self.no_data_timeout_reset;

        if self.connect_to_energy_meter() == True:
            super(RealSensor, self).__init__(
                                             gs_name = self.sensor_name,
                                             gs_id = 'bdeb5806-a9a2-11e6-a119-080027bc22fd',
                                             gs_description = 'Dual phase power meter',
                                             gs_interface = None,
                                             gs_read_interval = 60)
        else:
            logger.log('Energy meter device was not found on serial ports', 'error')
            raise SystemExit('Exiting')


    def SensorRead(self):
        if self.ser.inWaiting() > 0 :
            msg = self.ser.readline()
            measures = self.msg_parse(msg)
            logger.log(str(measures), 'debug')
            self.no_data_timeout = self.no_data_timeout_reset
            return measures
        else:
            if self.no_data_timeout > 0:
                self.no_data_timeout = self.no_data_timeout - 1
            else:
                logger.log('Energy meter device is not sending data... will try to reconnect', 'debug')
                self.no_data_timeout = self.no_data_timeout_reset
                self.reset_serial_port()
                res = self.connect_to_energy_meter()
                if res == True:
                    logger.log('Energy meter device is (re)connected', 'debug')
                else:
                    logger.log('Energy meter device fail to reconnect', 'error')

            return None

    def SensorDataProcessing(self,sensor_data):
        return sensor_data

    def connect_to_energy_meter(self):
        logger.log('Searching serial ports','info')
        ports = serial_ports()
        for port in ports:
            try:
                logger.log('Trying ' + str(port),'info')
                self.ser = serial.Serial(port, 115200, timeout=10)
                retry = 100
                while retry > 0:
                    logger.log('Countdown ' + str(retry),'info')
                    self.ser.flushInput()
                    self.ser.flushOutput()
                    self.ser.write('#')
                    check_em = self.ser.read()
                    if check_em == '!':
                        logger.log('Success on port ' + str(port),'info')
                        return True
                    retry = retry-1

            except (serial.SerialException) as e:
                logger.log('Energy meter device serial port exception', 'error')
                self.ser.close()
                print(e)
                return False

        return False

    def reset_serial_port(self):
        try:
            self.ser.close()
        except (serial.SerialException) as e:
            print(e)

    def msg_parse(self, msg):
        msg_p = msg.split()

        # checking message parsing result using list size
        if len(msg_p) != 6:
            return None

        measure = {}
        # for python 3 use:
        # measure['channel'] = int(str(msg_p[0]).replace("b'@", "").replace("'", ""))
        channel = int(str(msg_p[0]).replace("@", ""))
        if (channel != 1) and (channel != 2):
            return None

        measure['channel'] = channel
        measure['real_power'] = float(msg_p[1])
        measure['apparent_power'] = float(msg_p[2])
        measure['Vrms'] = float(msg_p[3])
        measure['Irms'] = float(msg_p[4])
        measure['power_factor'] = float(msg_p[5])

        return self.msg_agregate(measure)

    def msg_agregate(self, msg):
        if msg['channel'] == 1:
            self.message = {'ch1': msg}
            return None
        if msg['channel'] == 2:
            self.message['ch2'] = msg
            return self.message

if __name__ == '__main__':
    sensor = RealSensor()


    def do_exit(sig, stack):
        raise SystemExit('Exiting')

    signal.signal(signal.SIGINT, do_exit)
    signal.signal(signal.SIGUSR1, do_exit)

    signal.pause()
