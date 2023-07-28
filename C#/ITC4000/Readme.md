## Example Description
This example shows some of the functionality of ITC4001, ITC4002QCL, ITC4005, ITC4005QCL and ITC4020.
It demonstrates how to initialize the controller, make settings for the TEC controller, start PID auto tune and make settings for the laser diode controller.

## Instructions for Use

Before building and running this example. Please make sure you have downloaded the ITC4000 control app from here: 
https://www.thorlabs.de/software_pages/ViewSoftwarePage.cfm?Code=4000_Series

1) Set the project platform under the properties menu. This should be set to match the intended development platform e.g. x64 for deployment on 64-bit machines. 

2) Add the TL4000 library as a reference by right clicking the References section of the Solution Explorer. Navigate to the appropriate folder for your platform target: 
    * 32-bit: C:\Program Files (x86)\IVI Foundation\VISA\VisaCom\Primary Interop Assemblies
    * 64-bit: C:\Program Files\IVI Foundation\VISA\VisaCom64\Primary Interop Assemblies
This solution is pre-built for 64-bit systems, so it may be needed to delete and re-add the reference to this dll. 
