# Included Examples

## CLD Compact Laser Diode Controller Open and Control

This sample code shows how you can control a Thorlabs CLD laser diode controller in Python. It uses the PyVISA library to send SCPI commands to the controller and make or read settings on it.

Please note that the resource name (“USB0::0x1313::…”) in the sample code needs to be adjusted to the used controller type and unit.

## ITC4000, LDC4000, and TED4000 Controllers

The ITC4000, LDC4000, and TED4000 controllers can be controlled in the same way as the CLD controllers. So you can use this example as a guideline for these controllers as well. You just need to adjust the used commands. Please refer to the respective manuals for more details about the commands available on these devices.

We will also add dedicated examples for these controllers in the future.
