# Included Examples

## DC4100 and DC4104 (DC410x) LED Controller Open and Control with PySerial

The sample code shows how you can control a Thorlabs DC410x LED controller in Python using PySerial. The PySerial library is used to send serial commands to the controller and make or read settings on it. The PySerial library needs to be installed separately on the computer.

Please note that the COM port in the sample code needs to be changed for the COM port assigned to the controller. This can best be seen in the Device Manager.

## Serial Commands for the DC410x Controllers

The FTDI driver for virtual COM port needs to be installed on the computer to use serial commands. This should normally be automatically installed when the software provided wit the DC4100/DC4104 is installed. If the driver is not used, the communication over the COM port of the FTDI driver needs to be manually configured:

- Baudrate 115200
- 8 Databits
- No Parity
- 1 Stopbit
- Hardware

Within the firmware of the DC4100/DC4104, the following commands can be used directly through the COM port. Commands having the field <ch> need to have the channel number specified by a number comprised between 0 and 3. <ch++> means that one can also use the value ‐1 to act on all channels.

A list of the commands and descriptions for them can be found below:
```
- CC <ch><int>  Sets a constant current value in milliamp. The value can be between 0 and 1000
- CC? <ch>  Reads out and return the set current value
- L <ch><int> Sets a current limit value in milliamp. The value can be between 0 and 1000
- L? <ch> Reads out and return the set current limit value
- BP <ch++><float> Sets the brightness in %. The value can be comprised between 0.0 and 100.0
- BP? <ch> Reads out and return the brightness valule
- O <ch++><int> Switches a given LED on and off. 0 means off and 1 means on.
- O? <ch> Reads out and retunrs the on/off state of a LED
- A <ch++><int> Locks or unlocks a LED. 1 means that the LED is unlocked and 0 that it is locked
- A? <ch> Reads out and returns the lock/unlock state of a LED
- B <int> Sets the display brightness in %. The values are comprised between 0 and 100
- B? Reads out and retunrs the display brightness in %
- R? Reads and returns the status register
- N? Reads and returns the device name
- S? Reads and returns the device serial number
- ML <ch><int> Sets a maximum current limit value in milliamp. The value can be between 0 and 1000
- ML? <ch> Reads out and return the set maximum current limit value.
- V? Reads out and return the firmware version
- M <int> Sets the operation mode. 0 means constant current, 1 means constant birghtness and 2 means external control mode
- M? Reads out and return the actual operation mode
- H? Reads out and return the manufacturer name
- E? Reads out and return the alst error corde  
  Format:  ERROR <num>:text
  example: ERROR 0:No Error.
- HS? <ch> Reads out and return the serial number of the connected LED
- HN? <ch> Reads out and return the name of the connected LED
- CD? Reads out and return the calibration date
- WL? <ch> Reads out and return the <ch> wavelength of the connected LED
- FB? <ch> Reads out and return the <ch> forward voltage of the connected LED
- SM <int> Sets the saftey mode to „ON“ or „OFF“.
- SM? Reads out and return the safety mode settin
```
