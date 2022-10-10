# -*- coding: utf-8 -*-
"""
Example of TSP01 Rev. B in Python 3.10 (64-bit), using just the DLL file with ctypes.

See manual for description of what arguments do in the DLL functions.

Please note that this example only works with TSP01 units with "Rev. B" printed on the back.
"""

from ctypes import *

lib = cdll.LoadLibrary("C:\Program Files\IVI Foundation\VISA\Win64\Bin\TLTSPB_64.dll")

#Find out if there are devices connected.
deviceCount = c_ulong()
lib.TLTSPB_findRsrc(0, byref(deviceCount))

#If there are devices connected, determine their names.
if deviceCount.value >= 1:

    deviceName=create_string_buffer(256)
    
    #If there is only one device, it will be opened. Otherwise, ask which one should be connected.
    if deviceCount.value ==1:
        lib.TLTSPB_getRsrcName(0, 0, deviceName)
    else:
        print("Which device?")
        for i in range(deviceCount.value):
            lib.TLTSPB_getRsrcName(0, i, deviceName)
            print('#' + str(i+1) + " " + deviceName.value)
        device_num = input(">>>")
        lib.TLTSPB_getRsrcName(0, (device_num-1), deviceName)
        
    #Initialize the device.
    sessionHandle=c_ulong(0)
    lib.TLTSPB_init(deviceName, 0, 0, byref(sessionHandle))
    
    #Declare variables and constants for measurements
    #See TLTSP_Defines.h and TLTSPB.h for definitions of constants
    temperature=c_longdouble(0.0)
    humidity=c_longdouble(0.0)
    resistance = c_double(0.0)
    attribute = c_short(0)
    ch_intern = c_ushort(11)
    ch1_extern = c_ushort(12)
    ch2_extern = c_ushort(13)
    

    #Returns the temperature measured by the internal sensor in the TSP01 in °C.
    lib.TLTSPB_getTemperatureData(sessionHandle, ch_intern, attribute, byref(temperature))
    print ("Internal Sensor Temperature: " + str(round(temperature.value,2)) + " °C")

    #Channel 1
    #Check if the channel is connected by verifying the resistance is not zero.
    lib.TLTSPB_getThermRes(sessionHandle, ch1_extern, attribute, byref(resistance))
    if resistance.value > 0.0:
        #Returns the temperature measured by external sensor on Ch 1.
        lib.TLTSPB_measTemperature(sessionHandle, ch1_extern, byref(temperature))
        print("Ext. Ch 1 Temperature: " + str(round(temperature.value,2)) + " °C")

    resistance = c_double(0.0)

    #Same for Channel 2
    lib.TLTSPB_getThermRes(sessionHandle, ch2_extern , attribute, byref(resistance))
    if resistance.value > 0.0:
        lib.TLTSPB_measTemperature(sessionHandle, ch2_extern , byref(temperature))
        print("Ext. Ch 2 Temperature: " + str(round(temperature.value,2)) + " °C")

    #This returns the humidity measured by the internal sensor in the TSP01.
    lib.TLTSPB_getHumidityData(sessionHandle, attribute, byref(humidity))
    print("Humidity: " + str(round(humidity.value,2)) + " %")
    
    #Close the connection to the TSP01 Rev. B.
    lib.TLTSPB_close(sessionHandle)

else:
    print("No connected TSP01 Rev. B devices were detected. Check connections and installed drivers.")
