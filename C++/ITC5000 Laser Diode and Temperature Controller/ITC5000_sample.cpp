// Title: ITC5000 sample in C++. 
// Created Date: 2026 - 29 - 05
// Last modified date: 2026 - 29 - 05
// C++ Version Used: ISO C++ 14
// Thorlabs Driver Version: 1.0
// Notes: This example demonstrates how to initialize the controller, get information about the device and read settings.
// Tested with ITC5225

#include <stdio.h>
#include <iostream>
#include "TL5000.h"
using namespace std;

#define STDBUF_SIZE		512

static ViStatus printViError(ViSession vi, ViStatus err);

int main(int argc, char* argv[])
{
	ViStatus err;
	ViSession vi = 0;
	ViUInt32 resourceCount = 0;

	printf("TL5000 Example\n");

	err = TL5000_findRsrc(vi, &resourceCount);
	if (VI_SUCCESS == printViError(vi, err))
	{
		printf("TL5000_findRsrc() found %d devices\n", (int)resourceCount);
	}
	
	if (resourceCount > 0)
	{
		ViUInt32 i;
		ViChar resourceName[STDBUF_SIZE];
		ViChar modelName[STDBUF_SIZE] = "no model";
		ViChar serialNumber[STDBUF_SIZE] = "no S/N";
		ViChar manufacturer[STDBUF_SIZE] = "no manuf";
		ViBoolean resourceAvailable;

		i = 0;
		while (i < resourceCount)
		{
			err = TL5000_getRsrcName(vi, i, resourceName);
			if (VI_SUCCESS == printViError(vi, err))
			{
				printf("Found Resource Nr. %d:  \"%s\"\n", i + 1, resourceName);
			}

			err = TL5000_getRsrcInfo(vi, i, modelName, serialNumber, manufacturer, &resourceAvailable);
			if (VI_SUCCESS == printViError(vi, err))
			{
				printf("   Model:         \"%s\"\n", modelName);
				printf("   S/N:           \"%s\"\n", serialNumber);
				printf("   Manufacturer:  \"%s\"\n", manufacturer);
				printf("   Device free:   %d\n", (int)resourceAvailable);
			}

			i++;
		};


		ViSession instrHdl;
		cout << "Initializing instrument with resource name: " << resourceName << "\n";
		err = TL5000_init(resourceName, VI_TRUE, VI_TRUE, &instrHdl);
		if (err != VI_SUCCESS)
		{
			cout << "Error initializing instrument: " << err << "\n";
			return -1;
		}
		else
		{
			cout << "Instrument initialized successfully.\n\n";
		}

		//get settings
		ViUInt32 numSlots;
		ViChar devName[STDBUF_SIZE];
		ViChar devType[STDBUF_SIZE];
		ViChar firmwareRevision[STDBUF_SIZE];
		ViChar slotSerialNumber[STDBUF_SIZE];
		ViChar calMsg[STDBUF_SIZE];
		ViUInt32 tempSensor;
		ViUInt32 ldOperatingMode;
		ViReal64 ldCurrLimit;
		ViUInt32 ldPolarity;

		err = TL5000_getSlots(instrHdl, &numSlots);
		if (err) printViError(instrHdl, err);
		else printf("Number of slots: %d\n", numSlots);

		for (int i = 0; i < numSlots; i++) {
			err = TL5000_getSlotInfo(instrHdl, i, devName, devType, firmwareRevision, slotSerialNumber, calMsg);
			if (err) printViError(instrHdl, err);
			else {
				printf("Slot %d info:\n", i);
				printf("   Device Name: %s\n", devName);
				printf("   Device Type: %s\n", devType);
				printf("   Firmware Revision: %s\n", firmwareRevision);
				printf("   Serial Number: %s\n", slotSerialNumber);
				printf("   Calibration Message: %s\n\n", calMsg);
			}
		}

		err = TL5000_getTecTempSensor(instrHdl, 2, &tempSensor);
		if (err) printViError(instrHdl, err);
		else {
			switch (tempSensor)
			{
				case 0:
					cout << "TEC Temp Sensor: AD590 sensor\n";
					break;
				case 1:
					cout << "TEC Temp Sensor: low-range thermistor\n";
					break;
				case 2:
					cout << "TEC Temp Sensor: mid-range thermistor\n";
					break;
				case 3:
					cout << "TEC Temp Sensor: high-range thermistor\n";
					break;
				default:
					cout << "TEC Temp Sensor: unknown\n";
					break;
			}
		
		}

		TL5000_getLdcOperatingMode(instrHdl, 1, &ldOperatingMode);
		//This parameter returns the value TL5000_LD_OPMODE_CC(0) for constant current mode and TL5000_LD_OPMODE_CP(1) for constant power mode.
		if (err) printViError(instrHdl, err);		
		else{
							switch (ldOperatingMode)
				{
					case 0:
						cout << "LDC Operating Mode: constant current mode\n";
						break;
					case 1:
						cout << "LDC Operating Mode: constant power mode\n";
						break;
					default:
						cout << "LDC Operating Mode: unknown\n";
						break;
							}
		}

		TL5000_getLdcLdCurrLimit(instrHdl,1,0,&ldCurrLimit);
		if (err) printViError(instrHdl, err);
		else printf("LDC LD Current Limit: %f\n", ldCurrLimit);

	
		TL5000_getLdcLdPolarity(instrHdl, 1, &ldPolarity);
		if (err) printViError(instrHdl, err);
		else{
							switch (ldPolarity)
				{
					case 0:
						cout << "LDC LD Polarity: anode-grounded\n";
						break;
					case 1:
						cout << "LDC LD Polarity: cathode-grounded\n";
						break;
					default:
						cout << "LDC LD Polarity: unknown\n";
						break;
							}
		}
	

		TL5000_close(instrHdl);
		cout << "\nDevice disconnected.\n";
	}
	
	return 0;
}

static ViStatus printViError(ViSession vi, ViStatus err)
{
	ViStatus err2;
	ViChar errorMessage[STDBUF_SIZE];

	if (VI_SUCCESS == err)	return VI_SUCCESS;

	err2 = TL5000_error_message(vi, err, errorMessage);
	if (VI_SUCCESS == err2)
	{
		printf("Error message <%s>\n", errorMessage);
	}
	else
	{
		printf("TL5000_error_message failed and returned 0x%08X\n", (unsigned int)err2);
	}
	return err;
}


