import serial
from time import sleep
import tkinter
import tkinter as tk
import cv2
import PIL.Image, PIL.ImageTk
import PIL
import time
import numpy as np
import math

threshold1 = 85
threshold2 = 85
theta=0
r_width = 500
r_height = 300
minLineLength = 5
maxLineGap = 10
k_width = 5
k_height = 5
max_slider = 10
video_source = 4
global goInfinity
goInfinity = False
global trackLine
trackLine = False
currTime = 0
lastTime = 0
acc = 0
xpos = 0
velo = 0 
thetaR = 0

try:
        usb = serial.Serial("/dev/ttyACM0", 57600, timeout=5)
except:
        print("Error. Device not conected")
        exit()
        
usb.reset_input_buffer()
usb.write(b'begin 57600')
sleep(2)
usb.write(b'scan 2')
sleep(2)

def setSpeed(wheelR, wheelL):
        command = "wheel " + str(wheelR) + " " + str(wheelL)
        usb.write(command.encode())

class PID:
    def __init__(self, P=1.2, I=0.5, D=0.05, current_time):
        self.Kp = P
        self.Ki = I
        self.Kd = D

        self.sample_time = 10
        self.current_time = current_time if current_time is not None else time.time()
        self.last_time = self.current_time

        self.clear()

    def clear(self):
        self.SetPoint = 0.0

        self.PTerm = 0.0
        self.ITerm = 0.0
        self.DTerm = 0.0
        self.last_error = 0.0

        self.int_error = 0.0
        self.windup_guard = 20.0

        self.output = 0.0

    def update(self, feedback_value, currentTime:
      error = self.SetPoint - feedback_value

      self.current_time = currentTime
        
      delta_time = self.currentTime - self.last_time
      delta_error = error - self.last_error

      if (delta_time >= self.sample_time):
        self.PTerm = self.Kp * error
        self.ITerm += error * delta_time

        if (self.ITerm < -self.windup_guard):
          self.ITerm = -self.windup_guard
        elif (self.ITerm > self.windup_guard):
          self.ITerm = self.windup_guard

        self.DTerm = 0.0
        if delta_time > 0:
          self.DTerm = delta_error / delta_time

          # Remember last time and last error for next calculation
          self.last_time = self.currentTime
          self.last_error = error

          self.output = self.PTerm + (self.Ki * self.ITerm) + (self.Kd * self.DTerm)

    def setKp(self, proportional_gain):
        self.Kp = proportional_gain

    def setKi(self, integral_gain):
        self.Ki = integral_gain

    def setKd(self, derivative_gain):
        self.Kd = derivative_gain

    def setWindup(self, windup):
        self.windup_guard = windup

    def setSampleTime(self, sample_time):
        self.sample_time = sample_time


class App:
   def __init__(self, window, window_title):
      self.window = window
      self.window.title(window_title)
        
      self.Label = tk.Label(window, text = "Set Speed: ")
        
      self.button1 = tk.Button(window, text = 'STOP', width = 25, command = self.stop)
      self.button1.pack()
      
      self.button2 = tk.Button(window, text = 'Infinity', width = 25, command = self.switchInfinity)
      self.button2.pack()

      self.button3 = tk.Button(window, text = 'Line tracking', width = 25, command = self.switchLine)
      self.button3.pack()
      
      self.button4 = tk.Button(window, text = 'SIDE', width = 25, command = self.side)
      self.button4.pack() 
      # open video source (by default this will try to open the computer webcam
      # Create a canvas that can fit the above video source size //
      self.canvas = tkinter.Canvas(window, width = 1280, height = 720)
      self.canvas.pack()
 
      # After it is called once, the update method will be automatically called every delay milliseconds
      self.delay = 10
      self.update()
 
      self.window.mainloop()
   
   def update(self):
      if usb.in_waiting > 0:
         line1 = usb.readline().decode('utf-8')
         m1 = line1.find('\t')
         line2 = line1[m1+1:]
         m2 = line2.find('\t')
         line3 = line2[m2+1:]
         m3 = line3.find('\t')
         line4 = line3[m3+1:]
         m4 = line4.find('\t')
         currTime =line1[:m1]
         acc = line2[:m2]
         xpos =line3[:m3]
         velo = line4[:m4]
         thetaR =line4[m4+1:]
      
      ret, image = videoCap.read()
      # Convert the image to gray-scale
      gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
      # given input image, kernel width =5 height = 5, Gaussian kernel standard deviation
      blurred = cv2.GaussianBlur(gray, (k_width, k_height), 0)
      # Find the edges in the image using canny detector
      edged = cv2.Canny(blurred, threshold1, threshold2)
      # Detect points that form a line
      lines = cv2.HoughLinesP(edged,1,np.pi/180,max_slider,minLineLength,maxLineGap)
      #print(lines[0])
      theta=0
      if trackLine == True:
            for x in range(0, len(lines)):
                  for x1,y1,x2,y2 in lines[x]:
                        cv2.line(image,(x1,y1),(x2,y2),(255,0,0),3)
                        theta=theta+math.atan2((y2-y1),(x2-x1))
            print(theta)
            threshold=5
            if(theta>threshold):
               print("Go left")
            if(theta<-threshold):
               print("Go right")
            if(abs(theta)<threshold):
               print("Go straight")
            PID.update(theta, currTime)
            print(PID.update.output)
            PID.clear
            theta = 0
             
      #if goInfinity == True:
          
      
      # Get a frame from the video sour
      self.photo = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(edged))
      self.canvas.create_image(0, 0, image = self.photo, anchor = tkinter.NW)

      lastTime=currTime
 
      self.window.after(self.delay, self.update)
   
   def stop(self):
      print("STOP")
      global goInfinity
      goInfinity = False
      global trackLine
      trackLine = False
      setSpeed(0,0)
   def side(self):
      print("SIDE")
      setSpeed(-50,-35)      
   def switchInfinity(self):
      global goInfinity
      if not goInfinity:
            goInfinity = True
      else:
            goInfinity = False
   def switchLine(self):
      global trackLine
      if not trackLine:
            trackLine = True
      else:
            trackLine = False
 
# Create a window and pass it to the Application object
videoCap = cv2.VideoCapture(video_source)
if not videoCap.isOpened():
    raise ValueError("Unable to open video source", video_source)

App(tkinter.Tk(), "P5 Project")
   
