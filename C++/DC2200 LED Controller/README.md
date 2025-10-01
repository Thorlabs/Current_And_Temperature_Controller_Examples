## Example Description
This example shows some of the functionality of the DCC2200.
It demonstrates how to initialize the controller, get information about the controller and the LED, turn on and off the light and change the LED current.

## Instructions for Use

Before building and running this example. Please make sure you have downloaded the DC2200 control app from here: 
https://www.thorlabs.de/software_pages/ViewSoftwarePage.cfm?Code=DC2200

Guides written for these examples are written with Microsoft's Visual Studio in mind. Other IDEs can be used, but instructions are not provided in this repository.
1) Create a new VC++ project file or open the existed VC++ project file

2) Under the Solution Explorer, right click the Source Files, then add the DC2200_Example.cpp to the Source Files

3) Set the path of the header file according to the bit of the project you want to build:   
   a. Open Project\Properties\Configuration Properties\C/C++\General  
   b. Enter the path of the header files into Additional include Directories (**C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Include** for 32 bit or **C:\Program Files\IVI Foundation\VISA\Win64\include** for 64 bit)  

5) Set the path of the library according to the bit of the project you want to build:  
   a. Open Project\Properties\Configuration Properties\Linker\General  
   b. Enter the path of the library files into Additional Library Directories (**C:\Program Files (x86)\IVI Foundation\VISA\WinNT\lib\msc** for 32 bit or **C:\Program Files\IVI Foundation\VISA\Win64\Lib_x64\msc** for 64 bit)

7) Set additional depended library:  
   a. Open Project\Properties\Configuration Properties\Linker\Input  
   b. Enter the additional depended libraries into Additional Dependencies (**visa32.lib;TLDC2200_32.lib;** or **visa64.lib;TLDC2200_64.lib**);
