import serial   # serial imported for serial communication
import time     # used for delay functions
import pynput   # for keyboard listener, command shortcut functions

from pynput.mouse import Button, Controller
mouse = Controller()
from pynput.keyboard import Key, Controller
keyboard = Controller()

ArduinoSerial = serial.Serial('/dev/cu.smart_control-SPPDev',57600)     # create serial object called ArduinoSerial
time.sleep(2); # wait for 2 seconds for the communication to get established

while 1:
    incoming = ArduinoSerial.readline()
    data = str(incoming.decode('utf-8'))    # decode data
    # use 'if' because there is no 'switch-case' in Python!

    # swipe left
    if 'Normal LEFT' in data:
        keyboard.press(Key.left)
        keyboard.release(Key.left)

    # swipe right
    if 'Normal RIGHT' in data:
        keyboard.press(Key.right)
        keyboard.release(Key.right)

    # scroll up
    if 'Normal UP' in data:
        mouse.scroll(0,100)

    # scroll down
    if 'Normal DOWN' in data:
        mouse.scroll(0,-100)

    # escape and change to other opened tab
    if 'Normal NEAR' in data:
        keyboard.press(Key.esc)
        keyboard.release(Key.esc)
        with keyboard.pressed(Key.cmd):
                keyboard.press('h')
                keyboard.release('h')

    # play previous video (in a YouTube playlist)
    if 'Movie LEFT' in data:
        with keyboard.pressed(Key.shift):
                keyboard.press('p')
                keyboard.release('p')

    # play next video (in a YouTube playlist)
    if 'Movie RIGHT' in data:
        with keyboard.pressed(Key.shift):
                keyboard.press('n')
                keyboard.release('n')

    # turn on/off fullscreen
    if 'Movie UP' in data:
        keyboard.press('f')
        keyboard.release('f')

    # play/pause video
    if 'Movie DOWN' in data:
        keyboard.press('k')
        keyboard.release('k')

    # turn on/off subtitle (YouTube)
    if 'Movie NEAR' in data:
        keyboard.press('c')
        keyboard.release('c')

    # go to the right opened tab in web browser
    if 'A' in data:
        with keyboard.pressed(Key.cmd):
                keyboard.press(Key.alt)
                keyboard.press(Key.right)
                keyboard.release(Key.right)
                keyboard.release(Key.alt)

    # go to the left opened tab in web browser
    if 'B' in data:
        with keyboard.pressed(Key.cmd):
                keyboard.press(Key.alt)
                keyboard.press(Key.left)
                keyboard.release(Key.left)
                keyboard.release(Key.alt)

    # turn volume lower
    if 'C' in data:
        keyboard.press(Key.down)
        keyboard.release(Key.down)

    # turn volume higher
    if 'D' in data:
        keyboard.press(Key.up)
        keyboard.release(Key.up)

    # zoom in
    if 'E' in data:
        with keyboard.pressed(Key.cmd):
                keyboard.press('+')
                keyboard.release('+')       
    # zoom out
    if 'F' in data:
        with keyboard.pressed(Key.cmd):
                keyboard.press('-')
                keyboard.release('-')

    # fast-forward video
    if 'G' in data:
        keyboard.press('l')
        keyboard.release('l')
        
    # rewind video  
    if 'H' in data:
        keyboard.press('j')
        keyboard.release('j')


