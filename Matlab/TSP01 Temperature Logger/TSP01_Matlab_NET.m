%% Header
% Title: TSP01_Matlab_NET
% Created Date: 2023-11-27
% Last modified date: 2023-11-27
% Matlab Version:R2022a
% Thorlabs DLL version:1.0.262.119
%% Notes: The example connects to a TSP01 and reads temperature and humidity values
% The example uses methods from the .NET SDK 
%

clear all;
lib=NET.addAssembly('C:\Program Files\IVI Foundation\VISA\VisaCom64\Primary Interop Assemblies\Thorlabs.TLTSP01B_64.Interop.dll');
import Thorlabs.TLTSP01B_64.Interop.*;

%uncomment the following line to get an overview of the available methods
%methodsview('Thorlabs.TLTSP01B_64.Interop.TLTSPB')

handle = System.IntPtr(0);
device = TLTSPB(handle);

%Look for connected devices
try
    [~,deviceCount] = device.findRsrc();
catch
    disp( 'Unable to find compatible connected devices. ');
    return
end
disp(['Connected devices: ',num2str(deviceCount)]);

%connect to first available device
if deviceCount == 1
    deviceName=System.Text.StringBuilder(256);
    device.getRsrcName(0, deviceName);

    disp(['Connected device: ',char(deviceName.ToString)]);
    
    %reassign device to the selected TSP
    device = TLTSPB(deviceName.ToString(), false, false);

    [~,temperature] = device.getTemperatureData(12, 0);%First external temperature channel is channel 12
    disp(['Temperature for channel 1: ',num2str(temperature),'°C']);

    [~,humidity]=device.getHumidityData(0);
    disp(['Humidity of internal sensor: ',num2str(humidity)]);    
    
    %disconnect
    device.Dispose()

end
