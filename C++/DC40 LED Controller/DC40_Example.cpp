// Title:  DC40_Example
// Created Date: 2025-01-17
// Last modified date: 2025-01-17
// C++ Version Used: ISO C++ 14
// Thorlabs Software Version:	1.7
// Notes: This example demonstrates how to initialize the DC40 controller, get device information and
// turn on and off the output.  

#include <stdio.h>
#include <windows.h>
#include "TLDC.h"

/*===========================================================================
 Macros
===========================================================================*/

#ifndef VI_ERROR_RSRC_NFOUND
#define VI_ERROR_RSRC_NFOUND 111
#endif

/*===========================================================================
 Prototypes
===========================================================================*/
ViStatus find_instruments(ViChar** resource, ViBoolean* available);
void error_exit(ViSession instrHdl, ViStatus err);

ViStatus get_device_id(ViSession ihdl);

ViStatus get_ledInfo(ViSession ihdl);
ViStatus set_ledCurr(ViSession ihdl);
ViStatus reset_ledCurr(ViSession ihdl);
ViStatus set_ledOn(ViSession ihdl);
ViStatus set_ledOff(ViSession ihdl);

/*===========================================================================
 Functions
===========================================================================*/

//int main(int argc, char** argv)
int main()
{
    ViStatus    err;
    ViChar* rscPtr;
    ViSession   instrHdl = VI_NULL;
    ViBoolean   available;


    // Find resources
    if ((err = find_instruments(&rscPtr, &available)))  error_exit(VI_NULL, err);
    if (rscPtr == NULL) exit(EXIT_SUCCESS); // No instrument found
    if (0 == available)
    {
        printf("Found one device: \"%s\", but this device is not available!\n", rscPtr);
        printf("Maybe opened by another applicationm\n");
        exit(EXIT_SUCCESS); // No available instrument found
    }
    
       // Open session to DC Series instrument
    printf("Opening session to '%s' ...\n\n", rscPtr);
    if ((err = TLDC_init(rscPtr, VI_OFF, VI_OFF, &instrHdl))) error_exit(instrHdl, err);
    
    printf("Closing session to '%s' ...\n\n", rscPtr);
    err = TLDC_close(instrHdl);
    printf("Closing session to '%s' returned 0x%08X\n\n", rscPtr, err);

    printf("Re-Opening session to '%s' ...\n\n", rscPtr);
    err = TLDC_init(rscPtr, VI_OFF, VI_OFF, &instrHdl);
    printf("Re-Opening session to '%s' returned 0x%08X\n\n", rscPtr, err);
    
    err = get_device_id(instrHdl);
    if (err != VI_SUCCESS) {
        error_exit(instrHdl, err);
    }

    err = get_ledInfo(instrHdl);
    if (err!= VI_SUCCESS) {
        error_exit(instrHdl, err);
    }

    err = set_ledCurr(instrHdl);
    if (err != VI_SUCCESS) {
        error_exit(instrHdl, err);
    }

    err = set_ledOn(instrHdl);
    if (err != VI_SUCCESS) {
        error_exit(instrHdl, err);
    }
    Sleep(3000);
    err = set_ledOff(instrHdl);
    if (err != VI_SUCCESS) {
        error_exit(instrHdl, err);
    }
    printf("Closing session to device");
    if (instrHdl != VI_NULL) TLDC_close(instrHdl);

}

/*---------------------------------------------------------------------------
  Exit with error message
---------------------------------------------------------------------------*/
void error_exit(ViSession instrHdl, ViStatus err)
{
    ViChar buf[TLDC_ERR_DESCR_BUFFER_SIZE];

    // Print error
    TLDC_errorMessage(instrHdl, err, buf);
    fprintf(stderr, "ERROR: %s\n", buf);
    // Close instrument hande if open
    if (instrHdl != VI_NULL) TLDC_close(instrHdl);
    // Exit program
    exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------
  Find Instruments
---------------------------------------------------------------------------*/
ViStatus find_instruments(ViChar** resource, ViBoolean* available)
{
    ViStatus err;
    ViUInt32 deviceCount;
    static ViChar rsrcDescr[TLDC_BUFFER_SIZE];
    ViChar name[TLDC_BUFFER_SIZE], sernr[TLDC_BUFFER_SIZE];

    printf("Scanning for instruments ...\n");

    // prepare return value
    rsrcDescr[0] = '\0';
    *resource = rsrcDescr;

    err = TLDC_findRsrc(0, &deviceCount);
    switch (err)
    {
    case VI_SUCCESS:
        // At least one device was found. Nothing to do here. Continue with device selection menu.
        break;

    case VI_ERROR_RSRC_NFOUND:
        printf("No matching instruments found\n\n");
        return (err);

    default:
        return (err);
    }
      
    err = TLDC_getRsrcName(0, 0, rsrcDescr);//first device
    if (!err) err = TLDC_getRsrcInfo(0, 0, name, sernr, VI_NULL, available);

    return (err);    
}


/*---------------------------------------------------------------------------
 GET ID
---------------------------------------------------------------------------*/
ViStatus get_device_id(ViSession ihdl)
{
    ViStatus err;
    ViChar   nameBuf[TLDC_BUFFER_SIZE];
    ViChar   snBuf[TLDC_BUFFER_SIZE];
    ViChar   revFwBuf[TLDC_BUFFER_SIZE];
    ViChar   revDrvBuf[TLDC_BUFFER_SIZE];
    ViChar   calBuf[TLDC_BUFFER_SIZE];

    if ((err = TLDC_identificationQuery(ihdl, VI_NULL, nameBuf, snBuf, revFwBuf))) return(err);
    printf("Instrument:    %s\n", nameBuf);
    printf("Serial number: %s\n", snBuf);
    printf("Firmware:      V%s\n", revFwBuf);
    if ((err = TLDC_revisionQuery(ihdl, revDrvBuf, VI_NULL))) return(err);
    printf("Driver:        V%s", revDrvBuf);
    if ((err = TLDC_getCalibrationMsg(ihdl, calBuf))) return(err);
    printf("Cal message:   %s\n\n", calBuf);

    return VI_SUCCESS;
}


/*---------------------------------------------------------------------------
  Get LED Info
---------------------------------------------------------------------------*/
ViStatus get_ledInfo(ViSession ihdl)
{
    ViStatus err = VI_SUCCESS;
    ViChar ledName[30] = { "nothing read" };
    ViChar ledSerialNumber[30] = { "nothing read" };
    ViReal64 ledCurrentLimit = -1.0;
    ViReal64 ledForwardVoltage = -1.0;
    ViReal64 ledWavelength = 0.0;

    printf("Get LED Info ...\n");
    err = TLDC_getLedInfo(ihdl, ledName, ledSerialNumber, &ledCurrentLimit, &ledForwardVoltage, &ledWavelength);
    if (!err) printf("LED name: %s\n", ledName);
    printf("LED serial number: %s\n", ledSerialNumber);
    printf("LED current limit %.3lf A\n", ledCurrentLimit);
    printf("LED forward voltage %.3lf V\n", ledForwardVoltage);
    if (ledWavelength == -1)
        printf("LED is Warm White\n");
    else if (ledWavelength == -2)
        printf("LED is Neutral White\n");
    else if (ledWavelength == -3)
        printf("LED is Cold White\n");
    else
        printf("LED Wavelength %.3lf nm\n", ledWavelength);
    printf("\n\n");
    fflush(stdin);
    return (err);
}

/*---------------------------------------------------------------------------
  Set LED Current value
---------------------------------------------------------------------------*/
ViStatus set_ledCurr(ViSession ihdl)
{
    ViStatus err = VI_SUCCESS;
    ViReal64 ledCurr = 0.100;

    printf("Set LED Current to 100 mA ...\n");
    err = TLDC_setLedCurrentSetpoint(ihdl, ledCurr);
    if (!err) printf("LED Current set to: %.3lf A\r", ledCurr);
    printf("\n\n");
    fflush(stdin);
    return (err);
}


/*---------------------------------------------------------------------------
  Reset LED Current value
---------------------------------------------------------------------------*/
ViStatus reset_ledCurr(ViSession ihdl)
{
    ViStatus err = VI_SUCCESS;
    ViReal64 ledCurr = 0.000;

    printf("Reset LED Current to 0 mA ...\n");
    err = TLDC_setLedCurrentSetpoint(ihdl, ledCurr);
    if (!err) printf("LED Current reset to: %.3lf A\n", ledCurr);
    printf("\n\n");
    fflush(stdin);
    return (err);
}


/*---------------------------------------------------------------------------
  Set LED ON
---------------------------------------------------------------------------*/
ViStatus set_ledOn(ViSession ihdl)
{
    ViStatus err = VI_SUCCESS;

    printf("Set LED ON ...\n");
    err = TLDC_switchLedOutput(ihdl, VI_TRUE);
    if (!err) printf("LED is set ON\n");
    printf("\n\n");
    fflush(stdin);
    return (err);
}


/*---------------------------------------------------------------------------
  Set LED OFF
---------------------------------------------------------------------------*/
ViStatus set_ledOff(ViSession ihdl)
{
    ViStatus err = VI_SUCCESS;

    printf("Set LED OFF ...\n");
    err = TLDC_switchLedOutput(ihdl, VI_FALSE);
    if (!err) printf("LED is set OFF\n");
    printf("\n\n");
    fflush(stdin);
    return (err);
}


