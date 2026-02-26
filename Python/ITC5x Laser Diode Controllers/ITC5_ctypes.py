# -*- coding: utf-8 -*-

# Title: ITC5000 in Python with CTypes. 
# Created Date: 2026 - 02 - 17
# Last modified date: 2026 - 02 - 17
# Python Version Used: python3
# Thorlabs Driver Version: 1.0
# Notes: This example demonstrates how to control a Thorlabs ITC5 laser diode controller in Python using CTypes library. 


import time
from ctypes import *

lib = cdll.LoadLibrary("C:\Program Files\IVI Foundation\VISA\Win64\Bin\TL5000_64.dll")

#documentation: C:\Program Files (x86)\IVI Foundation\VISA\WinNT\TL5000\Manual 

#Find connected devices and get resource name of the first device found.
resourcecount=c_int(0)
lib.TL5000_findRsrc(0,byref(resourcecount))
print("Number of devices found: ", resourcecount.value)

resourcename=create_string_buffer(256)
lib.TL5000_getRsrcName(0,0,byref(resourcename))
print("Resource name: ", resourcename.value.decode())

#initialize device and get handle for further communication.
itc_handle=c_int(0)
res=lib.TL5000_init(resourcename,1,1,byref(itc_handle))

if res==0:
    print("Device initialized successfully")
    #TEC settings
    lib.TL5000_setTecTempSensor(itc_handle,2,0)#slot 2 (tec channel), 0(AD590 sensor)

    # Temperature Setpoint
    lib.TL5000_setTecTempSetpoint(itc_handle,2,c_double(26.0))
    tempset=c_double()
    lib.TL5000_getTecTempSetpoint(itc_handle,2,0,byref(tempset))
    print("Temperature setpoint ",tempset.value)

    #Switch on TEC output
    lib.TL5000_setTecOutputState(itc_handle,2,1)#slot 2 (tec channel), 1(on)

    time.sleep(2)

    #Measure temperature
    temperature=c_double(0)
    lib.TL5000_measTecTemp(itc_handle,2,0,byref(temperature))
    print("Measured temperature: ",temperature.value)

    #Laserdiode settings
    lib.TL5000_setLdcOperatingMode(itc_handle,1,0)#0: constant current mode, 1: constant power mode
    res=lib.TL5000_setLdcLdCurrSetpoint(itc_handle,1,c_double(0.1))# set current to 0.04 A

    time.sleep(1)

    setcurrent=c_double(0)
    res=lib.TL5000_getLdcLdCurrSetpoint(itc_handle,1,0,byref(setcurrent))
    print("Current is set to: ", setcurrent.value)

    #Close connection
    lib.TL5000_close(itc_handle)