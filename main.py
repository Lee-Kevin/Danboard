import logging
import time
import re
import serial
from threading import Thread, Event

from respeaker import Microphone

from respeaker import BingSpeechAPI
from respeaker import PixelRing,pixel_ring
BING_KEY = '95e4fe8b3a324389be4595bd1813121c'

ser = serial.Serial('/dev/ttyS1',115200,timeout=0)
data=[0xAA,0x01,0x64,0x55]
data1=[0xAA,0x01,0x00,0x55]
data2=[0xAA,0x01,0x00,0x55,0xAA,0x00,0x00,0x55]
data3=[0xAA,0x01,0x64,0x55,0xAA,0x00,0x64,0x55]
lefthand  = [0xAA,0x00,0x32,0x55]
righthand = [0xAA,0x01,0x32,0x55]
nodhead   = [0xAA,0x02,0x32,0x55]
shakehead = [0xAA,0x03,0x32,0x55]
wakeup    = [0xAA,0x02,0x64,0x55,0xAA,0x03,0x64,0x55]
origin = [lefthand,righthand,nodhead,shakehead]
def robot(action):
    if action == "LeftHand":
        data[1] = 0x00
        data1[1] = 0x00
        for i in range(0,3):
            ser.write(data)
            time.sleep(.4)
            ser.write(data1)
            time.sleep(.4)
        ser.write(lefthand)
    elif action == "RightHand":
        data[1] = 0x01
        data1[1] = 0x01
        for i in range(0,3):
            ser.write(data)
            time.sleep(.4)
            ser.write(data1)
            time.sleep(.4)
        ser.write(righthand)
    elif action == "NodHead":
        data[1] = 0x02
        data1[1] = 0x02
        for i in range(0,3):
            ser.write(data)
            time.sleep(.4)
            ser.write(data1)
            time.sleep(.4)  
        ser.write(nodhead)
    elif action == "ShakeHead":
        data[1] = 0x03
        data1[1] = 0x03
        for i in range(0,3):
            ser.write(data)
            time.sleep(.4)
            ser.write(data1)
            time.sleep(.4)
        ser.write(shakehead)
    elif action == "ShakeHand":
        for i in range(0,3):
            ser.write(data2)
            time.sleep(.5)
            ser.write(data3)
            time.sleep(.5)
    elif action == "WakeUp":
        ser.write(wakeup)
        time.sleep(.5)
    for i in range(0,4):
        ser.write(origin[i])

def task(quit_event):
    mic = Microphone(quit_event=quit_event)
    bing = BingSpeechAPI(BING_KEY)

    while not quit_event.is_set():
        if mic.wakeup('respeaker'):
            print('Wake up')
            pixel_ring.listen()
            robot("WakeUp")
            
            time.sleep(.1)
            data = mic.listen()
            
            try:
                pixel_ring.wait()
                text = bing.recognize(data, language='en-US')
                # spi.write('answer\n')
                print('\nBing:' + text.encode('utf-8'))      
                if re.search(r'shake', text) and re.search(r'left hand', text):
                    robot("LeftHand")
                    print("Shake Left hand")
                elif re.search(r'shake',text) and re.search(r'right hand',text):
                    robot("RightHand")
                    print("Shake right hand")
                elif re.search(r'shake.*(head).*',text):
                    robot("ShakeHead")
                    print("Shake head")
                elif re.search(r'head',text) or re.search(r'had',text):
                    robot("NodHead")
                    print("Nod head")
                elif re.search(r'hand',text) :
                    robot("ShakeHand")
                    print("Shake hand")
                elif re.search(r'hello',text):
                    robot("RightHand")
                    print("Hello")
                else:
                    print("Other")
            except Exception as e:
                print("\nCould not request results from Microsoft Bing Voice Recognition service; {0}".format(e))                
            # if text:
                # print('Recognized %s' % text)
            pixel_ring.off()

def main():
    logging.basicConfig(level=logging.DEBUG)
    
    quit_event = Event()
    thread = Thread(target=task, args=(quit_event,))
    thread.start()
    while True:
        try:
            time.sleep(1)
        except KeyboardInterrupt:
            print('Quit')
            quit_event.set()
            ser.close()
            break
    thread.join()

if __name__ == '__main__':
    main()