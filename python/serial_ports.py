import sys
import glob
import serial


def serial_ports():
    """ Lists serial port names - Linux Only!

    """
#     ports = glob.glob('/dev/tty[A-Za-z]*')
    ports = glob.glob('/dev/tty[ACM]*')
    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            print(s.portstr)
            s.close()
            result.append(port)
        except (serial.SerialException) as e:
            print(e)
    return result


if __name__ == '__main__':
    print(serial_ports())