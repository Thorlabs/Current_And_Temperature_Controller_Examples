% Title: upLED_NET.m
% Created Date: 2023-12-15
% Last modified date: 2023-12-15
% Matlab Version:R2022a
% Thorlabs DLL version:1.2.1588.335
%% Notes:The example connects to a UPLED device, changes the current setpoint and switches on light output
% The example uses methods from the .NET SDK 
%
clear all;
NET.addAssembly('C:\Program Files (x86)\Microsoft.NET\Primary Interop Assemblies\Thorlabs.TLUP_64.Interop.dll');
import Thorlabs.TLUP_64.Interop.*;

%Uncomment the next line to see an overview of the available functions
%methodsview('Thorlabs.TLUP_64.Interop.TLUP')

%Create a dummy TLUP object to check for compatible devices.
handle = System.IntPtr(0);
tlup = TLUP(handle);

%Look for connected devices
[~,deviceCount] = tlup.FindResource();

disp([num2str(deviceCount),' available devices']);

if deviceCount == 0
    disp('Unable to find compatible connected devices. Is the device connected, on, and using the TLUP driver? ');
    return
end

%connect to first device
deviceName=System.Text.StringBuilder(256);
if deviceCount == 1
    tlup.GetResourceName(0, deviceName);
end

%display device information
instrumentName=System.Text.StringBuilder(256);
serialNumber=System.Text.StringBuilder(256);
manufacturer=System.Text.StringBuilder(256);
tlup.GetResourceInformation(0, instrumentName, serialNumber,  manufacturer);
disp(manufacturer.ToString);
disp(instrumentName.ToString);
disp(serialNumber.ToString);

%initialize
device = TLUP(deviceName.ToString(), false, false);

%set current and enable output
device.Led_SetCurrentSetpoint(0.1);%current setpoint 0.1 A
device.Led_EnableOutput(true);%switch on LED
pause(2)

device.Led_EnableOutput(false);%switch off LED
device.Dispose();%disconnect
