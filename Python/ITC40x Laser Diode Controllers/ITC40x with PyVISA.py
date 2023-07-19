#Import the PyVISA library to Python.
import pyvisa
import sys

#Opens a resource manager and shows the available VISA devices.
rm = pyvisa.ResourceManager()


#Opens the connection to the device. The variable instr is the handle for the device.
#The 'USB0...' number for a device can e.g. be found in the returned list of rm.list_resources(). 
instr = rm.open_resource('USB0::0x1313::0x804A::M00443953::INSTR')

print("Used device: ",instr.query('*IDN?'))

#Sets the current setpoint and queries the new setting for confirmation.
#The command instr.write can be used to write data to the device when you do not expect a response from the device.
instr.write("source:current:level:amplitude 0.065")
print("Set LD current [A]:", instr.query("source:current:level:amplitude?"))

#These lines will close the resource manager and the handle to the device.
instr.close()
rm.close()
