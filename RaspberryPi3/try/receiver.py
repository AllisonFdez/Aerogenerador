#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Example program to receive packets from the radio link
#

#import RPi.GPIO as GPIO
#GPIO.setmode(GPIO.BCM)

from nrf24 import NRF24
import time
from time import gmtime, strftime
from datetime import datetime

pipes = [[0xe7, 0xd3, 0xf0, 0x35, 0xff], [0xe7, 0xd3, 0xf0, 0x35, 0xc2]]

radio2 = NRF24() #GPIO, spidev.SpiDev())
radio2.begin(0, 0, 25, 18)
radio2.setRetries(15,15)

radio2.setPayloadSize(32)
radio2.setChannel(0x4c)
radio2.setDataRate(NRF24.BR_1MBPS)
radio2.setPALevel(NRF24.PA_MAX)
radio2.setAutoAck(1)

radio2.openWritingPipe(pipes[0])
radio2.openReadingPipe(1, pipes[1])

radio.startListening()
radio.stopListening()

radio.printDetails()
radio.startListening()

while True:
    pipe = [0]
    while not radio.available(pipe, True):
        time.sleep(1000/1000000.0)
    recv_buffer = []
    radio.read(recv_buffer)
    out = ''.join(chr(i) for i in recv_buffer)
    print(out)
