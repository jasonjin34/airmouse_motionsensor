import serial # Serial imported for Serial Communication
import time # use for delay functions
import pynput
from pynput.mouse import Button, Controller
mouse = Controller()
from pynput.keyboard import Key, Controller
keyboard = Controller()

ArduinoSerial = serial.Serial('/dev/cu.smart_control-SPPDev',57600) #create serial object call arduinoserial
time.sleep(2); #wait for 2 seconds for the communication to get established

while 1:

    incoming = str (ArduinoSerial.readline())
    print (incoming)

    if 'Normal LEFT' in incoming:
        with keyboard.pressed(Key.ctrl):
                keyboard.press(Key.right)
                keyboard.release(Key.right)

    if 'Normal RIGHT' in incoming:
        with keyboard.pressed(Key.ctrl):
                keyboard.press(Key.left)
                keyboard.release(Key.left)

    if 'Normal UP' in incoming:
        mouse.scroll(0,100)

    if 'Normal DOWN' in incoming:
        mouse.scroll(0,-100)

    if 'Normal NEAR' in incoming:
        with keyboard.pressed(Key.cmd):
                keyboard.press(Key.tab)
                keyboard.release(Key.tab)

    if 'Movie LEFT' in incoming:
        with keyboard.pressed(Key.shift):
                keyboard.press('p')
                keyboard.release('p')

    if 'Movie RIGHT' in incoming:
        with keyboard.pressed(Key.shift):
                keyboard.press('n')
                keyboard.release('n')

    if 'Movie UP' in incoming:
        keyboard.press('f')
        keyboard.release('f')

    if 'Movie DOWN' in incoming:
        keyboard.press('k')
        keyboard.release('k')

    if 'Movie NEAR' in incoming:
        keyboard.press('c')
        keyboard.release('c')

    if 'Volumeup' in incoming:
        keyboard.press(Key.up)
        keyboard.release(Key.up)

    if 'Volumedown' in incoming:
        keyboard.press(Key.down)
        keyboard.release(Key.down)

    if 'Zoomout' in incoming:
        with keyboard.pressed(Key.cmd):
                keyboard.press('-')
                keyboard.release('-')

    if 'Zoomin' in incoming:
        with keyboard.pressed(Key.cmd):
                keyboard.press('+')
                keyboard.release('+')

    if 'Rewind' in incoming:
        keyboard.press('j')
        keyboard.release('j')

    if 'Fastforward' in incoming:
        keyboard.press('l')
        keyboard.release('l')
