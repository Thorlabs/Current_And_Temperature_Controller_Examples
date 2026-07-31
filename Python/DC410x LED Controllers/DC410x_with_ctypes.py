# -*- coding: utf-8 -*-

# Title: DC4100 in Python with CTypes. 
# Created Date: 2026 - 07 - 29
# Last modified date: 2026 - 07 - 29
# Python Version Used: Python 3.13
# Thorlabs DC4100 version: 2.5
# Notes: This example demonstrates how to control a Thorlabs DC4100 in Python using ctypes.

import time
from ctypes import *

def main():

    #Loading DLL file.
    lib = cdll.LoadLibrary(r"C:\Program Files\IVI Foundation\VISA\Win64\Bin\TLDC4100_64.dll")

    #Counting DC4100 devices.
    deviceCount = c_uint32()
    lib.TLDC4100_findRsrc(0,byref(deviceCount))
    if deviceCount.value > 0:
        print("Number of DC4100 devices found: " + str(deviceCount.value))
    else:
        print("No DC4100 devices found.")
        exit()
    print()

    #Reading model name and serial number of the first connected DC4100 device.
    RscName = create_string_buffer(256)
    lib.TLDC4100_getRsrcName(0, 0, RscName)

    #Initializing the first connected DC4100 device.
    DCHandle=c_int(0)
    res=lib.TLDC4100_init(RscName.value, 0, 0, byref(DCHandle))
    if res==0:
        print("connected")
    else:
        print("Connection failed")
        errorMessage=create_string_buffer(1024)
        lib.TLDC4100_error_message(DCHandle,res,byref(errorMessage))
        print(errorMessage.value.decode('utf_8'))
        
    #Make sure the LED is switched off (0 = off, 1 = on).
    lib.TLDC4100_setLedOnOff(DCHandle,-1,0)

    res=lib.TLDC4100_setOperationMode(DCHandle, 500)#0: constant current mode, 1: percental current mode, 2: external control mode

    lib.TLDC4100_setPercentalBrightness(DCHandle,1, 5) #Set brightness for second channel to 5% (0-100%)

    print("Switching on the second channel...")
    lib.TLDC4100_setLedOnOff(DCHandle,1,1)

    print("Wait...")
    time.sleep(2)

    print("Switching off the second channel...")
    lib.TLDC4100_setLedOnOff(DCHandle,1,0)
    
    lib.TLDC4100_close(DCHandle)
    print("Disconnected")

if __name__ == "__main__":
    main()