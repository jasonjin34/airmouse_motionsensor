import serial # Serial imported for Serial Communication
import time # use for delay functions
import pynput
from pynput.mouse import Button, Controller
mouse = Controller()
from pynput.keyboard import Key, Controller
keyboard = Controller()


ArduinoSerial = serial.Serial('com4',57600) #create serial object call arduinoserial
time.sleep(2); #wait for 2 seconds for the communication to get established

left_state = 0
right_state = 0

while 1:
    incoming = ArduinoSerial.readline()
    orginaldata = str(incoming.decode('utf-8'))
    #debug print
    #print(orginaldata)

    #split the string data value into 2 values
    #first check the lengh of the decode data 1 for the click variable
    if(len(orginaldata.split(":")) == 2):
        sx_value,sy_value= orginaldata.split(":")
        x_value = int(sx_value)
        y_value = int(sy_value)
        mouse.move(x_value,y_value)
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 999): #999 means click button
        mouse.click(Button.left,1)

    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 995): #up
        keyboard.press(Key.up)
        keyboard.release(Key.up)
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 996): #down
        keyboard.press(Key.down)
        keyboard.release(Key.down)
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 997 and left_state == 0): #left
        if(right_state == 1):
            keyboard.release(Key.right)
            right_state = 0
        keyboard.press(Key.left)
        left_state = 1
        
    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 998 and right_state == 0): #right
        if(left_state == 1):
            keyboard.release(Key.left)
            left_state = 0
        keyboard.press(Key.right)
        right_state = 1

    if(len(orginaldata.split(":")) == 1 and int(orginaldata) == 1000): #none
        keyboard.release(Key.left)
        keyboard.release(Key.right)
        keyboard.release(Key.up)
        keyboard.release(Key.down)

        
        
