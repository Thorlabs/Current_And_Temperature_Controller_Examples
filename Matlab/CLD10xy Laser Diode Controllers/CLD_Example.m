% Title: CLD1010_Example.m
% Created Date: 2024-03-09
% Last modified date: 2024-03-09
% Matlab Version:R2022a
% Thorlabs DLL version:3.1.0
% Notes:The example connects to a CLD1010 laser diode driver, changes the temperature setpoint and turns on TEC output
% Tested for CLD1010, firmware version 2.1.0
%
clc;
clear;

% Loading the dll and header file into MATLAB
libname='C:\Program Files\IVI Foundation\VISA\Win64\Bin\TL4000_64.dll';
hfile='C:\Program Files\IVI Foundation\VISA\Win64\Include\TL4000.h';
if (~libisloaded('TL4000_64'))
 loadlibrary(libname,hfile,'includepath','C:\Program Files\IVI Foundation\VISA\Win64\Lib_x64\msc', ...
     'includepath','C:\Program Files\IVI Foundation\VISA\Win64\Include','addheader', ...
     'C:\Program Files\IVI Foundation\VISA\Win64\Include\visa.h','addheader', ...
     'C:\Program Files\IVI Foundation\VISA\Win64\Include\vpptype.h');
end

% Uncomment the following line to displays the functions in the library
%libfunctionsview 'TL4000_64';

% Initialize device
resourceName=libpointer('int8Ptr',int8('USB0::0x1313::0x804F::M00456049::INSTR'));%change to correct serial number.
handle=libpointer('ulongPtr',0);
[a,b,c]=calllib('TL4000_64', 'TL4000_init', resourceName, 1, 0, handle);
disp(['Initialize device (0 = correct, rest = error): ', num2str(a)]);

% Make settings and turn on TEC output
if a==0
    calllib('TL4000_64', 'TL4000_setTempSetpoint', handle.value, 25.0);
    temperaturesetpoint=libpointer('doublePtr',0);
    calllib('TL4000_64', 'TL4000_getTempSetpoint', handle.value, 0, temperaturesetpoint);
    disp(['Temperature was set to ' ,num2str(temperaturesetpoint.value),'°C']);
    calllib('TL4000_64', 'TL4000_switchTecOutput',handle.value,true);
    pause(3);
    calllib('TL4000_64', 'TL4000_switchTecOutput',handle.value,false);
    % close connection to device
    calllib('TL4000_64', 'TL4000_close',handle.value);
end


