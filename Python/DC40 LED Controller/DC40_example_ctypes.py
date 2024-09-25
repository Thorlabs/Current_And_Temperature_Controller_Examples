# -*- coding: utf-8 -*-

# Title: DC40 in Python with CTypes. 
# Created Date: 2024 - 07 - 31
# Last modified date: 2024 - 07 - 31
# Python Version Used: Python 3.11
# Thorlabs DC40 version: 1.0.0
# Notes: This example demonstrates how to control a Thorlabs DC40 in Python using ctypes.

import time
from ctypes import *

def main():

    #Loading DLL file.
    lib = cdll.LoadLibrary("C:\Program Files\IVI Foundation\VISA\Win64\Bin\TLDC_64.dll")

    #Counting DC40 devices.
    deviceCount = c_uint32()
    lib.TLDC_findRsrc(0,byref(deviceCount))
    if deviceCount.value > 0:
        print("Number of DC40 devices found: " + str(deviceCount.value))
    else:
        print("No DC40 devices found.")
        exit()
    print()

    #Reading model name and serial number of the first connected DC40 device.
    RscName = create_string_buffer(256)
    serialNumber = create_string_buffer(256)
    lib.TLDC_getRsrcName(0, 0, RscName)

    #Initializing the first connected DC40 device.
    DCHandle=c_int(0)
    res=lib.TLDC_init(RscName.value, 0, 0, byref(DCHandle))
    if res==0:
        print("connected")

    #Enables the use of LEDs without EEPROM (non-Thorlabs or unmounted LEDs).
    lib.TLDC_setLedUseNonThorlabsLed(DCHandle, 1)
        
    #Make sure the LED is switched off (0 = off, 1 = on).
    lib.TLDC_switchLedOutput(DCHandle,0)

    #Get information about the connected LED.
    #!!! This only works for Thorlabs' Mounted LEDs with an EEPROM chip on the PCB.
    #!!! Third-party or unmounted LEDs do not have such a chip.
    LEDName=create_string_buffer(256)
    LEDSerialNumber=create_string_buffer(256)
    LEDCurrentLimit=c_double()
    LEDForwardVoltage=c_double()
    LEDWavelength=c_double(0)
    lib.TLDC_getLedInfo(DCHandle, LEDName, LEDSerialNumber, byref(LEDCurrentLimit),
                            byref(LEDForwardVoltage), byref(LEDWavelength))

    #if section: LED with EEPROM chip is connected
    if LEDWavelength.value != 0:
        print("EEPROM on LED detected.")
        print("Connected LED:", (LEDName.value).decode(), ", serial number:", (LEDSerialNumber.value).decode(),", wavelength [nm]:",LEDWavelength.value)
        print("max LED current [A]:", LEDCurrentLimit.value, ", max forward voltage [V]:", LEDForwardVoltage.value)
        print()

        #Set the current setpoint in Ampere to 10% of the current limit of this LED.
        currentSetpoint = c_double((0.1) * LEDCurrentLimit.value)
        lib.TLDC_setLedCurrentSetpoint(DCHandle,currentSetpoint)
        time.sleep(1)
        lib.TLDC_getLedCurrentSetpoint(DCHandle,0, byref(currentSetpoint))
        print("LED current set to [A]:", currentSetpoint.value)
        print()
        #else section: LED without EEPROM chip is connected
    else:
        print("No EEPROM on LED detected.")
        #Ask user to enter LED current setpoint and check if input is correct.
        try:
            currentSetpoint = c_double(0.001 * float(input("Please enter LED current setpoint in mA: ")))
        except:
            print("Error: Incorrect input. Please do not use letters, only use numbers.")
            print("Code will be stopped.")
            exit()
        lib.TLDC_setLedCurrentSetpoint(DCHandle, currentSetpoint)
        print("LED current set to", currentSetpoint.value*1000, "mA.")  

        
    #Switch the LED on, wait for 2 sec, then switch it off again.
    lib.TLDC_switchLedOutput(DCHandle,1)
    print("Switch LED on.")
    print("Wait...")
    time.sleep(2)
    lib.TLDC_switchLedOutput(DCHandle,0)
    print("Switch LED off.")
    
    
    #Connection to the DC40 device is closed.
    lib.TLDC_close(DCHandle)

if __name__ == "__main__":
    main()