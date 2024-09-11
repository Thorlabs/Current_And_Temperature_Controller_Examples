"""
Example Title: DC2200 with ctypes.py
Example Date of Creation(YYYY-MM-DD) 2022-02-16
Example Date of Last Modification on Github 2024-09-11
Version of Python used for Testing: Python 3.11, 64 bit
Version of the Thorlabs SDK used: DC2200 software 1.5
==================
Example Description: The example shows how to connect to a DC2200, turn on the LED in constant durrent and PWM mode and measure the current.
"""

#Import the necessary libraries to Python.
import ctypes
import os
import time
import sys

def main():

    # If you're using Python 3.7 or older change add_dll_directory to chdir
    if sys.version_info < (3, 8):
        os.chdir(r"C:\Program Files\IVI Foundation\VISA\Win64\Bin")
    else:
        os.add_dll_directory(r"C:\Program Files\IVI Foundation\VISA\Win64\Bin")

    #Load DLL library
    #os.add_dll_directory("C:\Program Files\IVI Foundation\VISA\Win64\Bin")
    library=ctypes.cdll.LoadLibrary("TLDC2200_64.dll")

    #Connect to device
    # !!! In the USB number the serial number (M00...) needs to be changed to the one of the connected device.
    devSession = ctypes.c_int()
    res=library.TLDC2200_init(b"USB0::0x1313::0x80C8::M00435922::INSTR",False,False,ctypes.byref(devSession))
    if res!=0:
        print("Connection failed")
        errorMessage=ctypes.create_string_buffer(1024)
        library.TLDC2200_error_message(devSession,res,ctypes.byref(errorMessage))
        print(errorMessage.value.decode('utf_8'))
    else:
        print("Device connected.")

        #Constant Current (CC) mode
        # Make CC settings
        library.TLDC2200_setOperationMode(devSession, 0)
        library.TLDC2200_setConstCurrent(devSession, ctypes.c_float(0.01))
        #Switch LED on
        library.TLDC2200_setLedOnOff(devSession, True)
        time.sleep(1)
        #Measure applied LED current
        current = ctypes.c_double()
        library.TLDC2200_get_led_current_measurement(devSession, ctypes.byref(current))
        print("Applied LED current: ", current.value)
        time.sleep(1)
        #Change LED current
        library.TLDC2200_setConstCurrent(devSession, ctypes.c_float(0.001))
        time.sleep(1)
        library.TLDC2200_get_led_current_measurement(devSession, ctypes.byref(current))
        print("Applied LED current: ", current.value)
        time.sleep(1)
        library.TLDC2200_setLedOnOff(devSession, False)

        #Pulse Width Modulation (PWM) mode
        #Make PWM settings
        library.TLDC2200_setOperationMode(devSession, 1)
        library.TLDC2200_setPWMCounts(devSession, 20)
        library.TLDC2200_setPWMCurrent(devSession, ctypes.c_float(0.01))
        library.TLDC2200_setPWMDutyCycle(devSession, 50)
        library.TLDC2200_setPWMFrequency(devSession, ctypes.c_double(10))
        #Switch LED on
        library.TLDC2200_setLedOnOff(devSession, True)
        time.sleep(2)
            
        #Close device connection
        library.TLDC2200_setLedOnOff(devSession, False)
        library.TLDC2200_close(devSession)

if __name__ == "__main__":
    main()