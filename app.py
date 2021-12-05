import serial
from time import sleep
import tkinter
import tkinter as tk
import cv2
import PIL.Image, PIL.ImageTk
import time

try:
        usb = serial.Serial("/dev/ttyACM0", 57600, timeout=5)
except:
        print("Error. Device not conected")
        exit()
        
usb.write(b'begin 57600')
sleep(2)
usb.write(b'scan 2')
sleep(2)

def setSpeed(wheelR, wheelL):
        command = "wheel " + str(wheelR) + " " + str(wheelL)
        usb.write(command.encode())

class App:
   def __init__(self, window, window_title, video_source=4):
      self.window = window
      self.window.title(window_title)
        
      self.Label = tk.Label(window, text = "Set Speed: ")
      self.Label.pack()
       
      self.button1 = tk.Button(window, text = 'FORWARD', width = 25, command = self.forward)
      self.button1.pack()
        
      self.button2 = tk.Button(window, text = 'BACKWARD', width = 25, command = self.backward)
      self.button2.pack()
        
      self.button3 = tk.Button(window, text = 'LEFT', width = 25, command = self.left)
      self.button3.pack()
        
      self.button4 = tk.Button(window, text = 'RIGHT', width = 25, command = self.right)
      self.button4.pack()
        
      self.button5 = tk.Button(window, text = 'STOP', width = 25, command = self.stop)
      self.button5.pack()
      
      self.video_source = video_source
 
      # open video source (by default this will try to open the computer webcam)
      self.vid = MyVideoCapture(self.video_source)
 
      # Create a canvas that can fit the above video source size //
      self.canvas = tkinter.Canvas(window, width = self.vid.width, height = self.vid.height)
      self.canvas.pack()
 
      # Button that lets the user take a snapshot
      self.btn_snapshot=tkinter.Button(window, text="Snapshot", width=50, command=self.snapshot)
      self.btn_snapshot.pack(anchor=tkinter.CENTER, expand=True)
 
      # After it is called once, the update method will be automatically called every delay milliseconds
      self.delay = 10
      self.update()
 
      self.window.mainloop()
   def snapshot(self):
      # Get a frame from the video source
      ret, frame = self.vid.get_frame()
 
      if ret:
         cv2.imwrite("frame-" + time.strftime("%d-%m-%Y-%H-%M-%S") + ".jpg", cv2.cvtColor(frame, cv2.COLOR_RGB2BGR))
 
   def update(self):
      # Get a frame from the video source
      ret, frame = self.vid.get_frame()
 
      if ret:
         self.photo = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(frame))
         self.canvas.create_image(0, 0, image = self.photo, anchor = tkinter.NW)
 
      self.window.after(self.delay, self.update)
   def forward(self):
                print("FORWARD")
                setSpeed(-250,-250)
   def backward(self):
                print("BACKWARD")
                setSpeed(25,25)
   def left(self):
                print("LEFT")
                setSpeed(-10,10)
   def right(self):
                print("RIGHT")
                setSpeed(10,-10)
   def stop(self):
                print("STOP")
                setSpeed(0,0)
 
 
class MyVideoCapture:
   def __init__(self, video_source=0):
      # Open the video source
      self.vid = cv2.VideoCapture(video_source)
      if not self.vid.isOpened():
         raise ValueError("Unable to open video source", video_source)
 
      # Get video source width and height
      self.width = self.vid.get(cv2.CAP_PROP_FRAME_WIDTH)
      self.height = self.vid.get(cv2.CAP_PROP_FRAME_HEIGHT)
 
   def get_frame(self):
      if self.vid.isOpened():
         ret, frame = self.vid.read()
         if ret:
         # Return a boolean success flag and the current frame converted to BGR
               return (ret, cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))
         else:
               return (ret, None)
      else:
         return (ret, None)
 
   # Release the video source when the object is destroyed
   def __del__(self):
      if self.vid.isOpened():
         self.vid.release()
 
# Create a window and pass it to the Application object
App(tkinter.Tk(), "P5 Project")
