# Title: CLD1010 SCPI in Python. 
# Created Date: 2022 - 02 - 16
# Last modified date: 2022 - 02 - 16
# Python Version Used: python3
# NI-VISA Driver Version: 24.0
# Notes: This example demonstrates how to control a Thorlabs CLD1010 controller in Python using SCPI commands. 

#Import the PyVISA library to Python.
import pyvisa

#Opens a resource manager and shows the available VISA devices.
rm = pyvisa.ResourceManager()
rm.list_resources()

#Opens the connection to the device. The variable instr is the handle for the device.
#The 'USB0...' number for a device can e.g. be found in the returned list of rm.list_resources(). 
instr = rm.open_resource('USB0::0x1313::0x804F::M00295819::INSTR')

#Returns the identification of the device.
#The command instr.query can be used when you want to get data from the device.
print("Used device:", instr.query("*IDN?"))

#Sets the current setpoint and queries the new setting for confirmation.
#The command instr.write can be used to write data to the device when you do not expect a response from the device.
instr.write("source1:current:level:amplitude 0.065")
print("Set LD current [A]:", instr.query("source1:current:level:amplitude?"))

#These lines will close the resource manager and the handle to the device.
instr.close()
rm.close()
