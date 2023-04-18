# Included Examples

## ITC40x with ctypes

This sample code shows how you can control a Thorlabs ITC laser diode controller in Python. It uses the CTypes library that has to be installed on the computer.

Please note that the resource name (“USB0::0x1313::…”) in the sample code needs to be adjusted to the used controller type and unit.

## ITC40x with PyVISA

This sample code shows how you can control a Thorlabs ITC laser diode controller in Python. It uses the PyVISA library that has to be installed on the computer.The PyVISA library is used to send SCPI commands to the controller.

Please note that the resource name (“USB0::0x1313::…”) in the sample code needs to be adjusted to the used controller type and unit.

## ITC4000, LDC4000, CLD1000 and TED4000 Controllers

The ITC4000, LDC4000, CLD1000 and TED4000 controllers can be controlled in the same way. So you can use this example as a guideline for these controllers as well. You just need to adjust the used commands. Please refer to the respective manuals for more details about the commands available on these devices.


