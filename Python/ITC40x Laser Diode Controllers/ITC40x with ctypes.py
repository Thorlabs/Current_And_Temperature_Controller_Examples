# -*- coding: utf-8 -*-

# Title: ITC4000 in Python with CTypes. 
# Created Date: 2023 - 04 - 19
# Last modified date: 2023 - 04 - 19
# Python Version Used: python3
# Thorlabs Driver Version: 3.1.0
# Notes: This example demonstrates how to control a Thorlabs ITC laser diode controller in Python using CTypes library. 

import os
import time
from ctypes import *

os.chdir(r"C:\Program Files\IVI Foundation\VISA\Win64\Bin")
lib = cdll.LoadLibrary("TL4000_64.dll")

itc_handle=c_int(0)

#documentation: C:\Program Files\IVI Foundation\VISA\Win64\TL4000\manual

#Start Scan- Resource name will need to be adjusted
#windows device manager -> NI-VISA USB Device -> Properties -> Details -> Device Instance ID
lib.TL4000_init(b"USB0::0x1313::0x804A::M00443953::INSTR",1,1,byref(itc_handle))

#TEC settings
lib.TL4000_setTempSensor(itc_handle,1)#0: AD590 sensor

lib.TL4000_setTecOperatingMode(itc_handle,1)#0: current source mode, 1: constant temperature mode

lib.TL4000_setTempUnit(itc_handle,0)#0: °Celsius, 1: Kelvin,  2: Fahrenheit

# Temperature Setpoint
lib.TL4000_setTempSetpoint(itc_handle,c_double(26.0))
tempset=c_double()
lib.TL4000_getTempSetpoint(itc_handle,0,byref(tempset))
print("Temperature setpoint ",tempset.value)

#Switch on TEC output
lib.TL4000_switchTecOutput(itc_handle,1)

time.sleep(2)

#Measure temperature
temperature=c_double(0)
lib.TL4000_measTemp(itc_handle,byref(temperature))
print("Measured temperature: ",temperature.value)

#Laserdiode settings
lib.TL4000_setLdOperatingMode(itc_handle,0)#0: constant current mode, 1: constant power mode

lib.TL4000_setLdCurrSetpoint(itc_handle,c_double(0.04))# set current to 0.04 A

time.sleep(1)

current=c_double(0)
lib.TL4000_getLdCurrSetpoint(itc_handle,0,byref(current))
print("Current is set to: ", current.value)

#Close connection
lib.TL4000_close(itc_handle)

