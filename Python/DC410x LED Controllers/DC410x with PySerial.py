# Title: DC410X in Python with Serial Command. 
# Created Date: 2023 - 03 - 13
# Last modified date: 2023 - 03 - 13
# Python Version Used: python3
# Notes: This example demonstrates how to control a Thorlabs DC410X controller in Python using serial commands. 

import serial
import time

#Make the COM port settings as required by the DC410x controllers.
baud_rate = 115200
data_bits = 8
stop_bits = 1
Parity = serial.PARITY_NONE
#Change this to the COM port of the DC410x controller.
#The COM port number can e.g. be seen in the device manager.
COM_port = "COM6"
#When all the settings are made, create a serial connection to COM port.
ser = serial.Serial(port = COM_port, baudrate = baud_rate, bytesize=data_bits, parity=Parity, stopbits=stop_bits,timeout=0.5)

#Write commands to the DC410x and read the response.
# Read and write details about the controller.
ser.write(b'N?\r\n')
print("Item number: ", (ser.readline()).decode('utf-8'))
ser.write(b'S?\r\n')
print("Serial number: ", (ser.readline()).decode('utf-8'))
ser.write(b'H?\r\n')
print("Manufacturer: ", (ser.readline()).decode('utf-8'))

#Operation modes are numbered like this:
#0: Constant current mode
#1: Constant brightness mode
#2: External control mode
#Set operation mode to constant current mode
ser.write(b'M 0\r\n')
ser.write(b'M?\r\n')
print("Operation modes: 0 = constant current, 1 = constant brightness, 2 = external control")
print("Set operation mode:", (ser.readline()).decode('utf-8'))


#Channel 0..3 for LEDs 1..4
#Set the current limit for channel 0 / LED 1 
#This is the max current which can be output by this channel.
ser.write(b'L 0 10\r\n')
ser.write(b'L? 0\r\n')
print("Current limit [mA] for LED 1:", (ser.readline()).decode('utf-8'))

#The current setpoint is the current which is acutally output by a channel to the LED.
#It needs to be lower than the current limit.
#Ask user to enter current setpoint.
try:
    CurrentSetpoint = int(input("Enter current setpoint [0 - 10 mA as an integer number] for LED 1: "))
except:
    print()
    print("Error: Incorrect input. Please use only integer numbers in the given range.")
    print("A current of 1 mA will be used in the following code.")
    CurrentSetpoint = 1
        
#Set the current setpoint for channel 0.
ser.write(b'CC 0 ' + bytes(str(CurrentSetpoint), 'utf-8') + b'\r\n')
ser.write(b'CC? 0\r\n')
print()
print("Current setpoint [mA] for LED 1:", (ser.readline()).decode('utf-8'))

#Switch LED 1 on channel 0 on and off.
ser.write(b'O 0 1\r\n')
print("LED switched on.")
time.sleep(2)
ser.write(b'O 0 0\r\n')
print("LED switched off.")

# Close and delete serial connection at the end of the program.
ser.close()
del ser
