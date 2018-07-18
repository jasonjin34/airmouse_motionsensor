import serial # Serial imported for Serial Communication
import time # use for delay functions
import pynput
from pynput.mouse import Button, Controller
mouse = Controller()
from pynput.keyboard import Key, Controller
keyboard = Controller()


ArduinoSerial = serial.Serial('com4',57600) #create serial object call arduinoserial
time.sleep(2); #wait for 2 seconds for the communication to get established
release_state = 0

while 1:
    incoming = ArduinoSerial.readline()
    orginaldata = str(incoming.decode('utf-8',"ignore"))
    
    #split the string data value into 2 values
    #first check the lengh of the decode data 1 for the click variable
    if(len(orginaldata.split(":")) == 2):
        sx_value,sy_value= orginaldata.split(":")
        x_value = int(sx_value)
        y_value = int(sy_value)
        mouse.move(x_value,y_value)
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 999): #999 means click button
        mouse.press(Button.left)
        mouse.release(Button.left)

    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 994): #game button
        keyboard.press('f')
        release_state = 0
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 995): #up
        keyboard.press('s')
        release_state = 0
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 996): #down
        keyboard.press('w')
        release_state = 0
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 997): #left
        keyboard.press('a')
        release_state = 0        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 998): #right
        keyboard.press('d')
        release_state = 0
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 1000 and release_state == 0): #none
        keyboard.release('w')
        keyboard.release('s')
        keyboard.release('a')
        keyboard.release('d')
        keyboard.release('f')
        release_state = 1
        

