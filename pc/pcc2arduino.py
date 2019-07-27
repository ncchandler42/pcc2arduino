import serial
import pygame
import sys
from time import sleep

def mapaxis(ax):
        return int(ax * 127 + 128)
class joy_grabber:
    bn = {'a': 0, 'b': 1, 'x': 2, 'y': 3, 'lb': 4, 'rb': 5, 'select': 6, 'start': 7, 'ls': 8, 'rs': 9}
    def __init__(self, id):
        self.joy = pygame.joystick.Joystick(id)
        self.joy.init()

        self.state = bytearray(10)
    
    def update(self):
        self.state[0] = ord('<')

        ###############################
        # FIRST BYTE
        ###############################
        b1 = 0
        if self.joy.get_hat(0)[1] == 1: # dpad_up
            b1 += 1
        elif self.joy.get_hat(0)[1] == -1: # dpad_down
            b1 += 2

        if self.joy.get_hat(0)[0] == -1: # dpad_left
            b1 += 4
        elif self.joy.get_hat(0)[0] == 1: # dpad_right
            b1 += 8

        b1 += self.joy.get_button(0) * 16  #a
        b1 += self.joy.get_button(1) * 32  #b
        b1 += self.joy.get_button(2) * 64  #x
        b1 += self.joy.get_button(3) * 128 #y

        self.state[1] = b1

        ###############################
        # SECOND BYTE
        ###############################
        b2 = 0

        b2 += self.joy.get_button(4) * 1 #lb
        b2 += self.joy.get_button(5) * 2 #rb

        if sys.platform == 'win32':
            if self.joy.get_axis(2) > 0.9: #lt
                b2 += 4
            elif self.joy.get_axis(2) < -0.9: #rt
                b2 += 8
        else:
            if self.joy.get_axis(2) > 0.9:
                b2 += 4
            if self.joy.get_axis(5) > 0.9:
                b2 += 8

        b2 += self.joy.get_button(6) * 16  #select
        b2 += self.joy.get_button(7) * 32  #start
        b2 += self.joy.get_button(8) * 64  #ls_press
        b2 += self.joy.get_button(9) * 128 #rs_press

        self.state[2] = b2

        ################################
        # Left and right sticks
        ################################
        self.state[3] = mapaxis(self.joy.get_axis(0)) #x_axis
        self.state[4] = mapaxis(self.joy.get_axis(1)) #y_axis

        if sys.platform == 'win32':
            trig = self.joy.get_axis(2)
            if trig > 0:
                self.state[5] = int(trig * 128)  #z_axis
            else:
                self.state[6] = int(trig * -128) #r_axis
        else:
            self.state[5] = mapaxis(self.joy.get_axis(2))
            self.state[6] = mapaxis(self.joy.get_axis(5))

        self.state[7] = mapaxis(self.joy.get_axis(3)) #u_axis
        self.state[8] = mapaxis(self.joy.get_axis(4)) #v_axis

        self.state[9] = ord('>')
    
    def send(self, ser):
        ser.write(self.state)
        ser.read()

    def kill(self):
        return self.joy.get_button(6) and self.joy.get_button(7)


# Code to determine serial port
port = ''
if sys.platform == 'win32':
    port = 'COM3'
elif sys.platform == 'linux':
    port = '/dev/ttyACM0'

if len(sys.argv) >= 2:
    port = sys.argv[1]


ser = serial.Serial(port, baudrate=115200)
sleep(2.5)

pygame.init()
pygame.joystick.init()

js = joy_grabber(0)

print('Reading joystick data. Press select and start to exit.')
while not js.kill():
    pygame.event.get()

    js.update()
    js.send(ser)
        
pygame.joystick.quit()
pygame.quit()
