## Example Description
This example shows some of the functionality of ITC5225.
It demonstrates how to initialize the controller, get information about the device and read settings.

## Instructions for Use

The ITC5225 device drivers have to be downloaded from the Thorlabs website and installed. 

Microsoft Visual Studio was used for the example. The following instructions will be different with other IDEs, but can be used as a guideline.

1) Create a new VC++ project file or open the existed VC++ project file

2) Under the Solution Explorer, right click the Source Files, then add the ITC5000_sample.cpp to the Source Files

3) Set the path of the header file according to the bit of the project you want to build:   
   a. Open Project\Properties\Configuration Properties\C/C++\General  
   b. Enter the path of the header files into Additional include Directories (**C:\Program Files(x86)\IVI Foundation\VISA\WinNT\include** or **C:\Program Files\IVI Foundation\VISA\Win64\include**)  

5) Set the path of the library according to the bit of the project you want to build:  
   a. Open Project\Properties\Configuration Properties\Linker\General  
   b. Enter the path of the library files into Additional Library Directories (**C:\Program Files(x86)\IVI Foundation\VISA\WinNT\lib\msc** or **C:\Program Files\IVI Foundation\VISA\Win64\Lib_x64\msc**)

7) Set additional depended library:  
   a. Open Project\Properties\Configuration Properties\Linker\Input  
   b. Enter the additional depended libraries into Additional Dependencies (**visa32.lib;TL4000_32.lib;** or **visa64.lib;TL4000_64.lib**);
