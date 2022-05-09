# Included Examples

## DC2200 LED Controller Open and Control with PyVISA and ctypes

These two sample codes show how you can control a Thorlabs DC2200 LED controller in Python using PyVISA or ctypes. The PyVISA library is used to send SCPI commands to the controller and make or read settings on it. As an alternative approach, the ctypes library can be used to load the DLL file for the DC2200 controller and use the functions implemented in the DLL. These libraries need to be installed separately on the computer.

Please note that the resource name (“USB0::0x1313::…”) in both sample codes needs to be adjusted to the used controller type and unit.
