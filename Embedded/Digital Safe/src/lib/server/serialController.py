import commands
import serial
import thread
import random
from time import sleep
from .webController import WebController

class CmdType():
    get_ip = 1
    set_ip = 2
    display_code = 3
    display_main = 4

class SerialController():
    def __init__(self):
        self.arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=0)
        self.cmd_list = {}
        self.cmd_list[CmdType.get_ip] = self.get_ip 

    def start(self):
        self.webcontroller = WebController()
        self.webcontroller.serialcontroller = self
        thread.start_new_thread(self.webcontroller.start,()) 
        while True:
            self.listen()
            sleep(0.03)

    def read(self):
        data = self.arduino.read()
        sleep(0.002)
        return data

    def get_packet(self):
        

    def listen(self):
        packet = ""
        if self.arduino.inWaiting() > 0:
            packet = self.get_packet()
        if self.validate(packet) == True:
           cmd = int(packet[3])
            if cmd in self.cmd_list:
                print "Running command: " + self.cmd_list[cmd].__name__
                self.cmd_list[cmd](packet)
            else:
                print "Invalid command code: " + cmd

    def get_ip(self, packet):
        octets = commands.getoutput("/sbin/ifconfig").split("\n")[1].split()[1].split(".")
        ip = octets[0].zfill(3) + "." + octets[1].zfill(3) + "." + octets[2].zfill(3) + "." + octets[3].zfill(3)
        print "IP Address: " + ip
        self.send(CmdType.set_ip, ip)

    def display_code(self):
        self.webcontroller.auth_code = random.randrange(1234567, 9999999)
        print "Running command: display_code"
        print "Auth Code: " + str(self.webcontroller.auth_code)
        self.send(CmdType.display_code, str(self.webcontroller.auth_code))

    def display_main_screen(self):
        self.send(CmdType.display_main, "")

    def send(self, cmdtype, data):
        byte_array = bytearray(chr(187) + chr(187) + chr(187) + chr(cmdtype) + chr(len(data)) + str(data) + chr(171) + chr(171) + chr(171))
        self.arduino.write(byte_array)

