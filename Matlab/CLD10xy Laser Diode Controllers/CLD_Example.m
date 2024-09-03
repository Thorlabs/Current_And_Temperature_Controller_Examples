% Title: CLD1010_Example.m
% Created Date: 2024-03-09
% Last modified date: 2024-03-09
% Matlab Version:R2022a
% Thorlabs DLL version:3.1.0
% Notes:The example connects to a CLD1010 laser diode driver, changes the temperature and setpoint and turns on TEC output
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
    disp(['Temperature setpoint:  ' ,num2str(temperaturesetpoint.value),'°C']);
    calllib('TL4000_64', 'TL4000_switchTecOutput',handle.value,true);
    
    % measure temperature
    temperature=libpointer('doublePtr',0);
    for i=1:5
        calllib('TL4000_64', 'TL4000_measTemp',handle.value,temperature);
        disp(['Temperature:' ,num2str(temperature.value),'°C']);
        pause(1);
    end

    % Make laser diode settings
    calllib('TL4000_64', 'TL4000_setLdOperatingMode', handle.value, 0);%0: constant current mode, 1: constant power mode
    calllib('TL4000_64', 'TL4000_setLdCurrSetpoint', handle.value, 0.04);%set current to 0.04 A
    currentsetpoint=libpointer('doublePtr',0);
    calllib('TL4000_64', 'TL4000_getLdCurrSetpoint', handle.value, 0, currentsetpoint);
    disp(['Current setpoint:  ' ,num2str(currentsetpoint.value),' A']);

    % close device
    calllib('TL4000_64', 'TL4000_switchTecOutput',handle.value,false);
    calllib('TL4000_64', 'TL4000_close',handle.value);
end


