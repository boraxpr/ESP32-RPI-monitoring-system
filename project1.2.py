#!/usr/bin/python3.7
import serial
import picamera
import time
from datetime import datetime
import sys
import os
if __name__ == '__main__':
    try:
    # Initialize serial communication
    # If error try to change ttyUSB0, ttyUSB1, ttyUSB2 as it depends on which usb port is connecting with ESP32
    # Besure to set baud rate to the same value on ESP32 code
        ser = serial.Serial('/dev/ttyUSB0', 57600)
    # Flush input, output buffer to avoid receiving incomplete/unuseful data 
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        ser.flush()
        print("Serial connection success!")
        
        #Picamera
        camera = picamera.PiCamera()
        #Set flip (Because camera is reverted upside down)
        camera.vflip = True
        camera.resolution = (1920, 1080)
        
        # Keep waiting for serial values
        while True:
            if ser.in_waiting > 0:
            
            # Decode serial value by unicode_escape and removing any spaces
                line = ser.readline().decode('unicode_escape').rstrip()
                number = [int(i) for  i in line.split() if i.isdigit()][0]
                
                # Print distance (From ESP32) value to terminal
                print("Distance: "+ str(number))
                
                # If distance < 60 cm (If someone is passing the ultrasonic module
                if(number < 60):
                #Get current date time to be used as file name for video footage
                    now = datetime.now()
                    dt_string = now.strftime("%d_%m_%y_%H_%M_%S")
                    #Clear serial values
                    ser.flush()
                    
                    #Record a video for 10 secs
                    camera.start_recording(dt_string+".h264")
                    time.sleep(10)
                    camera.stop_recording()
                    #Clear serial values
                    ser.flush()
                    
    except KeyboardInterrupt:
    # Flush input, output buffer before close
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        ser.close()
        sys.exit(0)
