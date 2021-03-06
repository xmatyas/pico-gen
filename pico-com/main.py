from time import sleep
import serial
import serial.tools.list_ports as list_ports
import argparse
from sys import argv

class PicoPacket:
    def __init__(self, size: int, type: chr, data: int):
        self._size = size
        self._type = type
        self._data = data
    def get_size(self) -> bytes:
        return self._size.to_bytes(1, 'big')
    def get_type(self) -> chr:
        return self._type
    def get_data(self) -> int:
        return self._data
    def get_data_as_array(self) -> bytearray:
        return int_to_list(self._data)

def int_to_list(num: int) -> list[int]:
    return [int(digit) for digit in str(num)]

def pico_connected():
    ports = list_ports.comports()
    print(hex(ports[0].vid), hex(ports[0].pid), ports[0].manufacturer, ports[0].product)
    for port in ports:
        if port.vid == 11914 and port.manufacturer == 'Raspberry Pi' and port.product == 'Pico':
            print('Found Pico device at port /{0}'.format(port.name))
            return True
    print('No Raspberry Pi Pico device found')
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

def serial_write(port, packet):
    if port.isOpen():
        #port.reset_input_buffer()
        #port.reset_output_buffer()
        sleep(0.5)
        port.write(packet.get_size())
        sleep(0.5)
        port.write(packet.get_type().encode('utf-8'))
        for digit in packet.get_data_as_array():
            sleep(0.25)
            port.write(digit.to_bytes(1,'big'))
        print("Bytearray values written : "+ str(len(packet.get_data_as_array())))
    else:
        raise serial.SerialException("Device disconnected before writing.")

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
        default='/dev/ttyUSB0',
        dest="port",
    )
    parser.add_argument(
        "-b", "--baud",
        help="Set assigned baudrate. If not set, defaults to 115200",
        default=115200,
        dest="baud",
    )
    parser.add_argument(
        "-d", "--duty",
        help="Set duty speed speed",
        dest="duty",
        type=int,
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
    #Opens the port
    args = parser.parse_args(argv[1:])
    if(not pico_connected()):
        exit()
    serial_port = init_serial(args.port)

    #Creates outgoing packet
    outgoingPacket = PicoPacket(len(str(abs(args.duty))) ,'r', abs(args.duty))
    serial_write(serial_port, outgoingPacket)
    print("Finished sending")
    serial_read(serial_port)

if __name__ == "__main__":
    main()