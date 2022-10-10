## Example Description
This example demonstrates how to initialize and pull measurement values from the TSP01 Rev. B. The code will also check for available external probes and take temperature measurements for these if available. If there are multiple TSP01 Rev. B units detected, you can select which one should be initialized.

This example is designed ONLY for the TSP01 Rev. B hardware. TSP01 units of this revision will have “Rev. B” written on the backside. 

## Instructions for Use

Before building and running this example, please make sure you have downloaded and installed the TSP software package from the Thorlabs website. The example code is looking for the DLL file in the default folder. If you have installed the DLLs to another folder, please make sure to adjust this line:

```
lib = cdll.LoadLibrary("C:\Program Files\IVI Foundation\VISA\Win64\Bin\TLTSPB_64.dll")
```
