# Included Examples

## upLED LED controller and upTEMP temperature logger with ctypes in Python 3.10

This sample code shows how you can control a Thorlabs upLED LED controller and a upTEMP temperature logger in Python using ctypes. The code can detect if an upLED or upTEMP is connected. If an upLED is connected, it will read data from the connected LED (if a mounted LED made by Thorlabs is used), it will adjust the current and switch the LED on and off. If an upTEMP is connected, it will configure the temperature measurement for the connected sensors and take measurements.

The code uses the ctypes library which can be used to load the DLL file for the upSeries controllers and use the functions implemented in the DLL. The ctypes library and the upSeries software need to be installed separately on the computer.