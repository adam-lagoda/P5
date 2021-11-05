import serial
from time import sleep


usb_port = "/dev/ttyACM0"

try: 
    usb = serial.Serial(usb_port, 57600, timeout=5)
except:
    print("error")
    exit()

usb.write(b'begin 57600')
sleep(3)
usb.write(b'scan 2')
sleep(3)

def setSpeed(wheelR, wheelL):
    command = f"wheel {wheelR} {wheelL}"
    usb.write(command.encode())
    if wheelR == 'stop' or wheelL == 'stop':
        usb.write(b'wheel 0 0')


while True:
    speed1 = input("Speed R: ")
    speed2 = input("Speed L: ")
    setSpeed(speed1,speed2)