#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Example program to receive packets from the radio link
#

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
from lib_nrf24 import NRF24
import time
import spidev

pipes = [[0xe7, 0xd3, 0xf0, 0x35, 0xff], [0xe7, 0xd3, 0xf0, 0x35, 0xc2]]

radio2 = NRF24(GPIO, spidev.SpiDev())
radio2.begin(0, 17)

radio2.setRetries(15,15)

radio2.setPALevel(NRF24.PA_MAX)

radio2.setAutoAck(True)
radio2.enableDynamicPayloads()
radio2.setCRCLength(16)
radio2.enableAckPayload()

radio2.openReadingPipe(1, pipes[0])
radio2.openReadingPipe(2, pipes[1])

radio2.startListening()

rpm = 0
xvec=[]
yvec=[]
zvec=[]

while True:
    try:
        recv_buffer = []
        radio2.read(recv_buffer, radio2.getDynamicPayloadSize())
        if recv_buffer[0] =='a':
            str_buf=''.join(recv_buffer)
            xvec.append(int(str_buf[1:4]))
            yvec.append(int(str_buf[4:7]))
            zvec.append(int(str_buf[7:10]))
            xvec.append(int(str_buf[10:13]))
            yvec.append(int(str_buf[13:16]))
            zvec.append(int(str_buf[16:19]))
            xvec.append(int(str_buf[19:22]))
            yvec.append(int(str_buf[22:25]))
            zvec.append(int(str_buf[25:28]))
        else:
            if recv_buffer[0] =='r':
                str_buf=''.join(recv_buffer)
                rpm=int(str_buf[1:5])
    except:
        print("No radio available yet.")
        time.sleep(2)
