// Title:  DC2200_Example
// Created Date: 2025-01-17
// Last modified date: 2025-01-17
// C++ Version Used: ISO C++ 14
// Thorlabs Software Version:	1.7
// Notes: This example demonstrates how to initialize the DC2200 controller, get device information, 
// turn on and off the output and change the current.  


#include <iostream>
#include <windows.h>
#include "TLDC2200.h"


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Global variables

ViSession m_handle;
#define DC2200_TERMINAL TLDC2200_OUTPUT_TERMINAL_10A

//==============================================================================
// Static functions

// prints the error message from an error code
static void print_error_msg(ViStatus errorCode)
{
	char messageBuffer[TLDC2200_ERR_DESCR_BUFFER_SIZE];

	// Get error string
	TLDC2200_error_message(m_handle, errorCode, messageBuffer);

	if ((errorCode & _VI_ERROR) == 0) // just a scan warning, no error
		printf("DC2200 Warning: %s\n", messageBuffer);

	else // errors
	{
		printf("DC2200 Error: %s\n", messageBuffer);

		// close instrument after an error has occured
		if (m_handle > 0)
			TLDC2200_close(m_handle);
	}

}

// returns a descriptive label string for the given terminal id
static const char* terminalLabel(ViUInt16 terminal)
{
	switch (terminal)
	{
	case TLDC2200_OUTPUT_TERMINAL_10A:  return "LED1 (10 A / 12 Pins)";
	case TLDC2200_OUTPUT_TERMINAL_2A:   return "LED2 (2 A / 4 Pins)";
	default: // do nothing
		break;
	}
	return "invalid terminal id!";
}

static const char* ledTypeLabel(ViInt32 ledType)
{
	switch (ledType)
	{
	case NO_HEAD:              return "no head";
	case FOUR_CHANNEL_HEAD:    return "four channel head";
	case ONE_CHANNEL_HEAD:     return "single channel head";
	case NOT_SUPPORTED_HEAD:   return "head with unsupported forward bias";
	case UNKNOWN_HEAD:         return "head with unknown type";
	case HEAD_WITHOUT_EEPROM:  return "old standard head";
	default: // do nothing
		break;
	}
	return "invalid LED type id!";
}

static const char* operationModeLabel(ViInt32 mode)
{
	switch (mode)
	{
	case MODUS_CONST_CURRENT:     return "constant current";
	case MODUS_PWM:               return "PWM";
	case MODUS_EXTERNAL_CONTROL:  return "external control";
	case MODUS_BRIGHTNESS:        return "brightness";
	case MODUS_PULSE:             return "pulse";
	case MODUS_INTERNAL_MOD:      return "internal modulation";
	case MODUS_TTL:               return "TTL";
	default: // do nothing
		break;
	}
	return "invalid operation mode id!";
}

//==============================================================================
// Global functions

int main(int argc, char* argv[])
{
	ViUInt32 deviceCount = 0;
	ViStatus res;

	ViChar devManufacturer[256];
	ViChar devModelName[256];
	ViChar devSerialNumber[256];
	ViBoolean devAvailable;
	ViChar devResourceString[256];
	ViChar ledModelName[256];
	ViChar ledSerialNumber[256];

	m_handle = 0;

	//==============================================================================
	// Resource functions
	//==============================================================================
 	res = TLDC2200_get_device_count(VI_NULL, &deviceCount);

	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}

	if (deviceCount == 0)
	{
		printf("No device connected\n");
		return 0;
	}

	// To keep it simple, this sample always opens the first device in the find list.
	res = TLDC2200_get_device_info(VI_NULL, 0, devManufacturer, devModelName, devSerialNumber, &devAvailable, devResourceString);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Manufacturer: %s\n", devManufacturer);
	printf("Model Name: %s\n", devModelName);
	printf("Serial Number: %s\n", devSerialNumber);
	printf("Device Available: %s\n", (devAvailable) ? "YES" : "NO");

	//==============================================================================
	// Utility functions
	//==============================================================================
	// connect with the first device
	res = TLDC2200_init(devResourceString, VI_TRUE, VI_FALSE, &m_handle);
	if ((res & _VI_ERROR) == _VI_ERROR)
	{
		print_error_msg(res);
		return 0;
	}


	res = TLDC2200_reset(m_handle);
	if (res == VI_SUCCESS)
	{
		printf("Device resetted.\n");
	}
	
	//==============================================================================
	// Configuration functions
	//==============================================================================
	ViUInt16 terminal;
	res = TLDC2200_get_output_terminal(m_handle, &terminal);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Active Terminal is: %s \n", terminalLabel(terminal));

	res = TLDC2200_test_head(m_handle, terminal);


	ViInt32 ledType;
	res = TLDC2200_get_head_info(m_handle, ledSerialNumber, ledModelName, &ledType);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	if (ledType == NO_HEAD) {
		printf("no head");
		print_error_msg(res);
		return 0;
	}

	printf("LED Head Parameters: \n");
	printf("LED Model: %s\n", ledModelName);
	printf("LED Serial Number: %s\n", ledSerialNumber);
	printf("LED Type: %s\n", ledTypeLabel(ledType));       

	ViInt32 operationMode;
	res = TLDC2200_getOperationMode(m_handle, &operationMode);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Operation Mode was: %d (%s) \n", operationMode, operationModeLabel(operationMode));

	if (MODUS_CONST_CURRENT != operationMode)
	{
		res = TLDC2200_setOperationMode(m_handle, MODUS_CONST_CURRENT);
		if (res != VI_SUCCESS)
		{
			print_error_msg(res);
			return 0;
		}
		printf("Operation Mode set to: %d (%s)\n", MODUS_CONST_CURRENT, operationModeLabel(MODUS_CONST_CURRENT));
	}

	ViReal64 minimumCurrent;
	ViReal64 maximumCurrent;
	ViUInt16 obsoleteTerminal = 0;
	ViReal64 obsoleteReal64 = 0.0;
	res = TLDC2200_get_limit_current_range(m_handle, obsoleteTerminal, obsoleteReal64, &minimumCurrent, &maximumCurrent);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Current Limit Range: min. %.2f A, max. %.2f A\n", minimumCurrent, maximumCurrent);

	ViReal32 limit;
	res = TLDC2200_getLimitCurrent(m_handle, &limit);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Current Limit was: %.2f A\n", limit);

	// set current limit set value to the maximum value possible by LED-head and console
	limit = maximumCurrent;
	res = TLDC2200_setLimitCurrent(m_handle, limit);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Current Limit set to: %.2f A\n", limit);

	//==============================================================================
	// Action/Status functions
	//==============================================================================
	ViReal32 current;
	res = TLDC2200_getConstCurrent(m_handle, &current);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("Current in Constant Current Mode: %.2f A\n", current);

	current = minimumCurrent;
	res = TLDC2200_setConstCurrent(m_handle, current);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("New Current set value: %.2f A\n", current);

	// turn the LED on
	res = TLDC2200_setLedOnOff(m_handle, VI_ON);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}
	printf("LED switched on. \n");

	// check if the LED is on
	ViBoolean ledState;
	res = TLDC2200_get_output_condition(m_handle, &ledState);
	if (res != VI_SUCCESS)
	{
		print_error_msg(res);
		return 0;
	}

	const int numSteps = 3;
	for (int i = 0; (i < numSteps) && (res == VI_SUCCESS); i++)
	{
		current = minimumCurrent + ((ViReal32)(i + 1) * (limit - minimumCurrent) / 10.0);
		res = TLDC2200_setConstCurrent(m_handle, current);
		printf("New Current set value: %.2f A\n", current);
		Sleep(5000);
	}

	// turn the LED off
	TLDC2200_setLedOnOff(m_handle, VI_OFF);
	printf("LED switched off. \n");

	TLDC2200_close(m_handle);


	return 0;
}



