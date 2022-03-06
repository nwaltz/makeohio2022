import numpy as np
import cv2
from PIL import Image
from PIL import ImageGrab
import pytesseract
import serial
import time
from time import sleep

#Create connection with arduino and give delay for arduino to sync
arduino = serial.Serial(port='COM3', baudrate=9600)
time.sleep(3)

#Give file location for pytesseract image-to-text converter
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

#Dictionary of keys holding the various distances to turn and boolean values of whether the distances are on the screen
dict = {"half":False, "quarter":False, "twoHundred":False}

#Function that grabs a screenshots the phone screen given the vertices of the pixels on the screen
def imageGrab(leftY, topX, rightY, bottomX):
    img = ImageGrab.grab(bbox=(leftY, topX, rightY, bottomX)) #x, y, w, h
    return np.array(img)

while True:
    #Continually screen grab the area of the screen that holds the distance until the next turn
    distanceImg_np = imageGrab(1043, 301, 1078, 316);
    
    #Converts screen grab to string to be checked with various distances in dictionary
    num = pytesseract.image_to_string(distanceImg_np)
    if "0.5" in num:
        dict["half"] = True
    elif ".2" in num or ",2" in num:
        dict["quarter"] = True
    elif "200" in num or "yiel" in num or "ylel" in num:
        dict["twoHundred"] = True

    #Run if one of the distances has been met in the screen grab
    if dict["half"] or dict["quarter"] or dict["twoHundred"]:
        #Screen grab sections of the turn signal on google maps and turn to grey scale
        turnLeftImg_np = imageGrab(1056, 276, 1065, 285)
        turnLeftImg_gs = cv2.cvtColor(turnLeftImg_np, cv2.COLOR_BGR2GRAY)

        turnRightImg_np = imageGrab(1067, 278, 1071, 279)
        turnRightImg_gs = cv2.cvtColor(turnRightImg_np, cv2.COLOR_BGR2GRAY)

        #Compare the turn signal screen grabs to write to the arduino if the user should turn left,
        #right or continue straight
        if turnLeftImg_gs[0,0] > 125:
            if dict["half"]:
                arduino.write(b'D')
                dict["half"] = False
            if dict["quarter"]:
                arduino.write(b'E')
                dict["quarter"] = False
            if dict["twoHundred"]:
                arduino.write(b'F')
                dict["twoHundred"] = False
            time.sleep(0.5)
        elif turnRightImg_gs[0,0] > 125:
            if dict["half"]:
                arduino.write(b'A')
                dict["half"] = False
            if dict.get("quarter"):
                arduino.write(b'B')
                dict["quarter"] = False
            if dict["twoHundred"]:
                arduino.write(b'C')
                dict["twoHundred"] = False
            time.sleep(5)
    #Destroy all created windows before running through the loop again
    cv2.destroyAllWindows()

#Close arduino communication
arduino.close()