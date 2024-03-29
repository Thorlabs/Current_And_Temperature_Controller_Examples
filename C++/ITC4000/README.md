## Example Description
This example shows some of the functionality of ITC4001, ITC4002QCL, ITC4005, ITC4005QCL and ITC4020.
It demonstrates how to initialize the controller, make settings for the TEC controller, and make settings for the laser diode controller.

## Instructions for Use

Before building and running this example. Please make sure you have downloaded the ITC4000 control app from here: 
https://www.thorlabs.de/software_pages/ViewSoftwarePage.cfm?Code=4000_Series

Guides written for these examples are written with Microsoft's Visual Studio in mind. Other IDEs can be used, but instructions are not provided in this repository.
1) Create a new VC++ project file or open the existed VC++ project file

2) Under the Solution Explorer, right click the Source Files, then add the ITC4000.cpp to the Source Files

3) Set the path of the header file according to the bit of the project you want to build:   
   a. Open Project\Properties\Configuration Properties\C/C++\General  
   b. Enter the path of the header files into Additional include Directories (**C:\Program Files(x86)\IVI Foundation\VISA\WinNT\include** or **C:\Program Files\IVI Foundation\VISA\Win64\include**)  

5) Set the path of the library according to the bit of the project you want to build:  
   a. Open Project\Properties\Configuration Properties\Linker\General  
   b. Enter the path of the library files into Additional Library Directories (**C:\Program Files(x86)\IVI Foundation\VISA\WinNT\lib\msc** or **C:\Program Files\IVI Foundation\VISA\Win64\Lib_x64\msc**)

7) Set additional depended library:  
   a. Open Project\Properties\Configuration Properties\Linker\Input  
   b. Enter the additional depended libraries into Additional Dependencies (**visa32.lib;TL4000_32.lib;** or **visa64.lib;TL4000_64.lib**);
