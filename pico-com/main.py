from time import sleep
import serial
import serial.tools.list_ports as list_ports
import argparse
from sys import argv

class PicoPacket:
    def __init__(self):
        self.header
        self.type
        self.data

def pico_connected():
    ports = list_ports.comports()
    print(hex(ports[0].vid), hex(ports[0].pid), ports[0].manufacturer, ports[0].product)
    for port in ports:
        if port.vid == 11914 and port.manufacturer == 'Raspberry Pi' and port.product == 'Pico':
            print('Found Pico device at port /{0}'.format(port.name))
            return True
        else:
            print('No device found')
            return False

"""
TESTING: UART DEVICES DON'T ALWAYS HAVE READABLE DESCRIPTION
def uart_connected():
    ports = list_ports.comports()
    for port in ports:
        if port.manufacturer == '':
            print('Found UART device connected to serial port {}'.format(port.name))
"""

def init_serial(port=None, baudrate=115200, bytesize=serial.EIGHTBITS, 
                parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, 
                timeout=None ):
    serial_port = serial.Serial(port, baudrate, bytesize, parity, stopbits, timeout)
    return serial_port

def serial_read(port):
    msg = ''
    if not port.isOpen():
        raise serial.SerialException("Device disconnected before reading.")
    trigger = False
    while port.in_waiting > 0:
        msg += port.read().decode('utf-8', errors='ignore')
        trigger = True
    if trigger is True:
        print(msg)

def serial_write(port, data):
    if not port.isOpen():
        raise serial.SerialException("Device disconnected before writing.")
    port.write(data.encode('utf-8'))

def main():
    """main program"""
    parser = argparse.ArgumentParser(
        prog = "main.py",                  #TODO: edit this
        usage = "%(prog)s [flags] [keyword]",
        description="Serial communication with RPi Pico",
    )
    experimental = parser.add_argument_group('experimental arguments')
    parser.add_argument(
        "-p", "--port",
        help="Assign a serial port connection",
        default=None,
        dest="port",
    )
    parser.add_argument(
        "-b", "--baud",
        help="Set assigned baudrate. If not set, defaults to 115200",
        default=115200,
        dest="baud",
    )
    parser.add_argument(
        "-r", "--rpm",
        help="Set RPM speed",
        dest="rpm",
    )
    experimental.add_argument(
        "--bytesize",
        help="EXPERIMENTAL | Sets size of bits to transfer",
        dest="bytesize",
    )
    experimental.add_argument(
        "--stopbits",
        help="EXPERIMENTAL | Sets stopbits parameter",
        dest="stopbits",
    )
    experimental.add_argument(
        "--parity",
        help="EXPERIMENTAL | Sets parity bit size",
        dest="parity",
    )
    experimental.add_argument(
        "--timeout",
        help="EXPERIMENTAL | Sets timeout",
        dest="timeout",
    )
    #opens the port
    #TODO: add serial path as argparse, add optional baud rate parse, etc.
    serial_port = init_serial(port='/dev/ttyUSB0')
    args = parser.parse_args(argv[1:])
    while True:
        for letter in args.rpm:
            serial_write(serial_port, letter)
            serial_read(serial_port)
            sleep(0.5)                      #For readability reasons
    print("Finished sending")
    exit()

if __name__ == "__main__":
    main()