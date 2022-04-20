
## Example Description
This examples demonstrates how to initialize and pull measurement values from the TSP01 Rev B. The code will also check for available external probes and take temperature measurements for these if available. 

## Instructions for Use

Before building and running this example. Please make sure you have downloaded the KLC control app from here: https://www.thorlabs.com/software_pages/viewsoftwarepage.cfm?code=KLC101

1) Set the project platform under the properties menu. This should be set to match the intended development platform e.g. x64 for deployment on 64-bit machines. 

2) Copy the KLC library to the project build location (the default location will be in the bin/debug/ folder). Both the 32 and 64-bit dll's will be found in:  
    * C:\Program Files (x86)\Thorlabs\KLC\Sample\Thorlabs_KLC_C++SDK\
This solution is pre-built for 64-bit systems, so the dllimport will need to be modified for the 32-bit library name as needed. 
