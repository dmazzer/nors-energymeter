#!/usr/bin/env python2

""" 
sensor_power.py: Current and Voltage sensor driver

This sensor driver collects current and voltage measurements from external arduino board.
"""
import time

__author__ = "Daniel Mazzer"
__copyright__ = "Copyright 2016, NORS project"
__credits__ = ""
__license__ = "MIT"
__maintainer__ = "Daniel Mazzer"
__email__ = "dmazzer@gmail.com"

import sys
import signal
import serial

# sys.path.append('./genericsensor/')
# sys.path.append('./norsutils/')
# sys.path.append('../../')

from genericsensor.genericsensor import Nors_GenericSensor

from grovepi import *

from norsutils.logmsgs.logger import Logger

from serial_ports import serial_ports

logger = Logger('debug')
logger.log("SENSOR - Dual phase power meter", 'info')

class RealSensor(Nors_GenericSensor):
    def __init__(self):

        self.sensor_name = 'PWRMTR'
        
        if self.connect_to_energy_meter() == False:
            logger.log('Energy meter device was not found on serial ports', 'error')
            return False
        
        print('Deu certo')
        
        super(RealSensor, self).__init__(
                                         gs_name = self.sensor_name,
                                         gs_id = 'bdeb5806-a9a2-11e6-a119-080027bc22fd',
                                         gs_description = 'Dual phase power meter', 
                                         gs_interface = None,
                                         gs_pull_interval = 15, 
                                         gs_read_interval = 14)

        
    def SensorRead(self):
        if self.ser.in_waiting > 0 :
            msg = self.ser.readline()
            measures = self.msg_parse(msg)
            print(str(measures))

        return measures

    def SensorDataProcessing(self,sensor_data):
        return sensor_data
    
    def connect_to_energy_meter(self):
        ports = serial_ports()
        for port in ports:
            try:
                self.ser = serial.Serial(port, 115200, timeout=5)
                self.ser.flushInput()
                self.ser.flushOutput()
                self.ser.write('#')
                time.sleep(2)
                check_em = self.ser.read()
                if check_em == '!':
                    return True
                
            except (serial.SerialException) as e:
                return False
        
        return False

    def msg_parse(self, msg):
        msg_p = msg.split()
        measure = {}
        # for python 3 use:
        # measure['channel'] = int(str(msg_p[0]).replace("b'@", "").replace("'", ""))
        measure['channel'] = int(str(msg_p[0]).replace("@", ""))
        measure['real_power'] = float(msg_p[1])
        measure['apparent_power'] = float(msg_p[2]) 
        measure['Vrms'] = float(msg_p[3]) 
        measure['Irms'] = float(msg_p[4]) 
        measure['power_factor'] = float(msg_p[5])
        return measure 



if __name__ == '__main__':
    sensor = RealSensor()


    def do_exit(sig, stack):
        raise SystemExit('Exiting')
    
    signal.signal(signal.SIGINT, do_exit)
    signal.signal(signal.SIGUSR1, do_exit)
    
    signal.pause()    

    
