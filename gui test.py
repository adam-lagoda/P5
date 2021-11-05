import serial
from time import sleep
from tkinter import Tk, Label, Button

# usb_port = "/dev/ttyACM0"

# try: 
#     usb = serial.Serial(usb_port, 57600, timeout=5)
# except:
#     print("error")
#     exit()

# usb.write(b'begin 57600')
# sleep(3)
# usb.write(b'scan 2')
# sleep(3)

# def setSpeed(wheelR, wheelL):
#     command = f"wheel {wheelR} {wheelL}"
#     usb.write(command.encode())
#     if wheelR == 'stop' or wheelL == 'stop':
#         usb.write(b'wheel 0 0')

class P5GUI:
    def __init__(self, master):
        self.master = master
        master.title("P5 Project Interface")

        self.label = Label(master, text="Set Speed: ")
        self.label.pack()

        self.set_speed_100 = Button(master, text="100", command=self.setSpeed100)
        self.set_speed_100.pack()

        self.set_speed_0 = Button(master, text="0", command=self.setSpeed0)
        self.set_speed_0.pack()

    def setSpeed100(self):
        print("100")
        #usb.write(b'wheel 265 300')
    def setSpeed0(self):
        print("0")
        #usb.write(b'wheel 0 0')

root = Tk()
my_gui = P5GUI(root)
root.mainloop()