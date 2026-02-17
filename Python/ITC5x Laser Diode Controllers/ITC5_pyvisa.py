# Title: ITC5000 SCPI in Python. 
# Created Date: 2026 - 02 - 17
# Last modified date: 2026 - 02 - 17
# Python Version Used: python3
# NI-VISA Driver Version: 2025
# Notes: This example demonstrates how to control a Thorlabs ITC5 laser diode controller in Python using SCPI commands. 

#Import the PyVISA library to Python.
import pyvisa
import sys

#Opens a resource manager and shows the available VISA devices.
rm = pyvisa.ResourceManager()


#Opens the connection to the device. The variable instr is the handle for the device.
#The 'USB0...' number for a device can e.g. be found in the returned list of rm.list_resources(). 
instr = rm.open_resource('USB0::0x1313::0x8281::M01299293::INSTR')

print("Used device: ",instr.query('*IDN?'))

#Sets the current setpoint and queries the new setting for confirmation.
#The command instr.write can be used to write data to the device when you do not expect a response from the device.
#instr.write("source:current:level:amplitude 0.065")
instr.write("SOURce1:CURRent 0.05")#laser diode current set to 50mA
print("Set LD current [A]:", instr.query("SOURce1:CURRent?"))

#These lines will close the resource manager and the handle to the device.
instr.close()
rm.close()