# -*- coding: utf-8 -*-

"""
Example of C Libraries for TLUP in Python 3 with CTypes

This example will detect all connected upSeries devices.
It will connect to the first detected device and will perform some steps depending on the device.
Currently the upLED and the upTEMP are supported in this example.
"""

import os
import time
from ctypes import *

#Loading DLL file.
lib = cdll.LoadLibrary("C:\Program Files\IVI Foundation\VISA\Win64\Bin\TLUP_64.dll")

#Counting upSeries devices.
deviceCount = c_uint32()
lib.TLUP_findRsrc(0,byref(deviceCount))
if deviceCount.value > 0:
    print("Number of upSeries devices found: " + str(deviceCount.value))
else:
    print("No upSeries devices found.")
    exit()
print()

#Reading model name and serial number of the first connected upSeries device.
modelName = create_string_buffer(256)
serialNumber = create_string_buffer(256)
lib.TLUP_getRsrcInfo(0, 0, modelName, serialNumber, 0, 0)
print("Connecting to this device:")
print("Model name: ", (modelName.value).decode(), ", Serial number: ", (serialNumber.value).decode())
print()

#Initializing the first connected upSeries device.
upName = create_string_buffer(256)
lib.TLUP_getRsrcName(0, 0, upName)
upHandle=c_int(0)
res=lib.TLUP_init(upName.value, 0, 0, byref(upHandle))

#If the device is an upLED, this section is executed.
if (modelName.value).decode() == "upLED":

    #Make sure the LED is switched off (0 = off, 1 = on).
    lib.TLUP_switchLedOutput(upHandle,0)

    #Get information about the connected LED.
    #!!! This only works for Thorlabs' Mounted LEDs.
    currentSetpoint = c_double()
    LEDName = create_string_buffer(256)
    LEDSerialNumber = create_string_buffer(256)
    LEDCurrentLimit = c_double()
    LEDForwardVoltage = c_double()
    LEDWavelength = c_double()
    
    lib.TLUP_getLedInfo(upHandle, LEDName, LEDSerialNumber, byref(LEDCurrentLimit),
                        byref(LEDForwardVoltage), byref(LEDWavelength))
    print("Connected LED:", (LEDName.value).decode(), ", serial number:", (LEDSerialNumber.value).decode(),", wavelength [nm]:",LEDWavelength.value)
    print("max LED current [A]:", LEDCurrentLimit.value, ", max forward voltage [V]:", LEDForwardVoltage.value)
    print()

    #Set the current setpoint in Ampere to 10% of the current limit of this LED.
    currentSetpoint = c_double((0.1) * LEDCurrentLimit.value)
    lib.TLUP_setLedCurrentSetpoint(upHandle,currentSetpoint)
    print("LED current set to [A]:", currentSetpoint.value)
    print()

    #Switch the LED on, wait for 2 sec, then switch it off again.
    lib.TLUP_switchLedOutput(upHandle,1)
    print("Switch LED on.")
    print("Wait...")
    time.sleep(2)
    lib.TLUP_switchLedOutput(upHandle,0)
    print("Switch LED off.")
    
#If the device is an upTEMP, this section is executed.
elif (modelName.value).decode() == "upTEMP":

    #Configure which type of temperature sensor is connected (here: thermistor)
    #
    #TLUP_TEMP_SENS_8TH  (1): 8 thermistor channels are available
    #TLUP_TEMP_SENS_8TC  (2): 8 thermocouple channels are available
    #TLUP_TEMP_SENS_4RTD (3): 4 resistance temperature device (RTD) (e.g. Pt100) channels are available
    lib.TLUP_setTempSensConfig(upHandle, c_int(1))

    #Configure which temperature unit is used (here: Celsius)
    #
    #TLUP_TEMP_U_KELVIN  (0): Kelvin
    #TLUP_TEMP_U_CELSIUS (1): Celsius
    #TLUP_TEMP_U_FAHREN  (2): Fahrenheit
    lib.TLUP_setTempUnit(upHandle, c_int(1))

    #Configure the characteristics of the thermistor
    #
    #Here the T0, R0, beta parameters of the thermistor are known and are used to determine the corresponding Steinhart-Hart coefficients.
    t0 = c_double(298.15)
    r0 = c_double(10000.0)
    beta = c_double(3900.0)
    
    steinhartHartParameterA = c_double()
    steinhartHartParameterB = c_double()
    steinhartHartParameterC = c_double()
    steinhartHartParameterD = c_double()
    steinhartHartParameterE = c_double()
    steinhartHartParameterF = c_double()    

    lib.TLUP_getSteinhartHartParameterFromExponentialParameterConvertion(
        upHandle, t0, r0, beta,
        byref(steinhartHartParameterA), byref(steinhartHartParameterB),
        byref(steinhartHartParameterC), byref(steinhartHartParameterD),
        byref(steinhartHartParameterE), byref(steinhartHartParameterF))

    """#Get possible temperature ranges for these Steinhart-Hart-Parameters.
    minimumRanges = (c_double*8)()
    maximumRanges = (c_double*8)()
    lib.TLUP_getTemperatureRangesFromSteinhartHartParameters(
        upHandle,
        steinhartHartParameterA, steinhartHartParameterB,
        steinhartHartParameterC, steinhartHartParameterD,
        steinhartHartParameterE, steinhartHartParameterF,
        minimumRanges, maximumRanges)

    for r in range(8):
        print("Range", r+1, "minimum:", minimumRanges[r], "maximum:", maximumRanges[r])

    print()"""

    #Channel 1 is configured with the calculated Steinhart-Hart coefficients.
    CH1 = c_int(1)
    lib.TLUP_setThermSensConfig(upHandle, CH1, c_int(3),
                                steinhartHartParameterA, steinhartHartParameterB,
                                steinhartHartParameterC, steinhartHartParameterD,
                                steinhartHartParameterE, steinhartHartParameterF)

    """#Channel list is created and enabled.
    on = c_ubyte(1)
    off = c_ubyte(0)
    chanList = (c_ubyte*8)()
    chanList[0] = on
    chanList[1] = off
    chanList[2] = off
    chanList[3] = off
    chanList[4] = off
    chanList[5] = off
    chanList[6] = off
    chanList[7] = off
    lib.TLUP_setMeasChanList(upHandle, chanList)"""

    #Temperature measurements are enabled.
    lib.TLUP_setTempMeasurementState(upHandle, c_int(1))
    
    #Temperature measurements are performed and new temperature values are printed to the screen.
    temperature = c_double()
    isNew = c_bool()
    for x in range(1000):
        lib.TLUP_measTempTemperature (upHandle, CH1, byref(temperature), byref(isNew))
        if isNew:
            print("Measured temperature: ", temperature.value)

    
#If the device is neither an upLED nor an upTEMP, this section is executed.
else:
    print("upSeries device type is not yet supported by this example.")
    
#Connection to the upSeries device is closed.
lib.TLUP_close(upHandle)
