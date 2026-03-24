% Title: DC2200_NET_NET.m
% Created Date: 2026-03-24
% Last modified date: 2026-03-24
% Matlab Version:R2023a
% Thorlabs DLL version:1.6.2181.41
%% Notes:The example connects to a DC2200 device, changes the current setpoint and switches on light output
% The example uses methods from the .NET SDK 
%
clear all;
NET.addAssembly('C:\Program Files (x86)\Microsoft.NET\Primary Interop Assemblies\Thorlabs.TLDC2200_64.Interop.dll');
import Thorlabs.TLDC2200_64.Interop.*;

%Uncomment the next line to see an overview of the available functions
%methodsview('Thorlabs.TLDC2200_64.Interop.TLDC2200')

%Create a dummy TLDC2200 object to check for compatible devices.
handle = System.IntPtr(0);
tldc2200 = TLDC2200(handle);

%Look for connected devices
[~,deviceCount] =tldc2200.get_device_count();
disp([num2str(deviceCount),' available devices']);

if deviceCount == 0
    disp('Unable to find compatible connected devices. Is the device connected, on, and using the TLUP driver? ');
    return
end

%display device information
instrumentName=System.Text.StringBuilder(256);
serialNumber=System.Text.StringBuilder(256);
manufacturer=System.Text.StringBuilder(256);
resourcename=System.Text.StringBuilder(256);
tldc2200.get_device_info(0, manufacturer,instrumentName,serialNumber,resourcename);
%tldc2200.GetResourceInformation(0, manufacturer,modelname,);
disp(manufacturer.ToString);
disp(instrumentName.ToString);
disp(serialNumber.ToString);

%initialize
device = TLDC2200(resourcename.ToString(), false, false);

%set current and enable output
device.setOperationMode(0);
device.setConstCurrent(0.01);
device.setLedOnOff(true);%switch on LED
pause(2)

device.setLedOnOff(false);%switch off LED

device.Dispose();%disconnect
