
## Example Description
This examples demonstrates how to initialize and pull measurement values from the TSP01 Rev B. The code will also check for available external probes and take temperature measurements for these if available. 

## Instructions for Use

Before building and running this example. Please make sure you have downloaded the TSP control app from here: https://www.thorlabs.com/software_pages/viewsoftwarepage.cfm?code=TSP

This example is designed ONLY for the TSP01 Rev B hardware. Your hardware version can be checked by inspecting the back of the TSP unit. Any Rev B hardware will be indicated above the serial number. 

1) Set the project platform under the properties menu. This should be set to match the intended development platform e.g. x64 for deployment on 64-bit machines. 

2) Add the TLTSPB library as a reference by right clicking the References section of the Solution Explorer. Navigate to the appropriate folder for your platform target: 
    * 32-bit: C:\Program Files (x86)\IVI Foundation\VISA\VisaCom\Primary Interop Assemblies
    * 64-bit: C:\Program Files\IVI Foundation\VISA\VisaCom64\Primary Interop Assemblies
This solution is pre-built for 64-bit systems, so it may be needed to delete and re-add the reference to this dll. 

