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
radio2.begin(8,25)

radio2.setRetries(15,15)

radio2.setPALevel(NRF24.PA_MAX)

radio2.setAutoAck(True)
radio2.enableDynamicPayloads()
radio2.setCRCLength(16)
radio2.enableAckPayload()

radio2.openReadingPipe(1, pipes[0])
radio2.openReadingPipe(2, pipes[1])

radio2.startListening()

while True:
    try:
        recv_buffer = []
        radio2.read(recv_buffer, radio2.getDynamicPayloadSize())
        print("Reading.")
    except:
        print("No radio available yet.")
        time.sleep(2)
