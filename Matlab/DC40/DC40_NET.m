% Title: DC40_NET.m
% Created Date: 2024-08-ß2
% Last modified date: 2024-08-ß2
% Matlab Version:R2022a
% Thorlabs software version:1.0.0
%% Notes:The example connects to a DC40 device, changes the current setpoint and switches on light output
% The example uses methods from the .NET SDK 
%
clear all;
NET.addAssembly('C:\Program Files (x86)\Microsoft.NET\Primary Interop Assemblies\Thorlabs.TLDC_64.Interop.dll');
import Thorlabs.TLDC_64.Interop.*;

%Uncomment the next line to see an overview of the available functions
%methodsview('Thorlabs.TLDC_64.Interop.TLDC')

%Create a dummy TLDC object to check for compatible devices.
handle = System.IntPtr(0);
tlDC = TLDC(handle);

%Look for connected devices
[~,deviceCount] = tlDC.findRsrc();

disp([num2str(deviceCount),' available devices']);

if deviceCount == 0
    disp('Unable to find compatible connected devices. Is the device connected, on, and using the TLDC driver? ');
    return
end

%connect to first device
deviceName=System.Text.StringBuilder(256);
if deviceCount == 1
    tlDC.getRsrcName(0, deviceName);
end

%display device information
instrumentName=System.Text.StringBuilder(256);
serialNumber=System.Text.StringBuilder(256);
manufacturer=System.Text.StringBuilder(256);
tlDC.getRsrcInfo(0, instrumentName, serialNumber,  manufacturer);
disp(manufacturer.ToString);
disp(instrumentName.ToString);
disp(serialNumber.ToString);

%initialize
device = TLDC(deviceName.ToString(), false, false);

%set current and enable output
device.setLedCurrentSetpoint(0.1);%current setpoint 0.1 A
device.switchLedOutput(true);%switch on LED
pause(2)

device.switchLedOutput(false);%switch off LED
device.Dispose();%disconnect
