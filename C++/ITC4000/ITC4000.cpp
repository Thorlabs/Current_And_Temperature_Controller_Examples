// Title: ITC4000 in C++. 
// Created Date: 2023 - 11 - 09
// Last modified date: 2023 - 11 - 09
// C++ Version Used: ISO C++ 14
// Thorlabs Driver Version: 3.1.0
// Notes: This example demonstrates how to initialize the controller, make settings for the TEC controller, 
//and make settings for the laser diode controller.

#include <iostream>
#include <cstring>
#include <vector>
#include <stdlib.h>
using namespace std;

//include device specific
#include "TL4000.h"

/*===========================================================================
 Resource Name and Basic Settings
===========================================================================*/
ViChar resourceName[256] = "USB::4883::32842::M00262781::INSTR"; //change to correct serial number. It can be found in the "Remote Control" in Menu Screen.
ViInt16 tempSensor = 1;//(0): AD590 sensor, (1): Thermistor (low range),  (2): Thermistor (high range),  (3): PT100 RTD sensor, (4): PT1000 RTD sensor,  (5): LM35 sensor, (6): LM335 sensor
ViInt16 ldOperatingMode = 0;//(0): Constant current mode, (1): Constant power mode
ViReal64 ldCurrLimit = 0.25;//current unit: A
ViInt16 ldPolarity = 1;//(0): cathode grounded (CG);  (1): anode grounded(AG)
ViInt16 pdPolarity = 0;//(0): cathode grounded (CG);  (1): anode grounded(AG)
/*===========================================================================
 Prototypes
===========================================================================*/
void ErrorExit(ViSession handle, ViStatus err);
void Error(ViSession handle, ViStatus err);
bool IsNumber(string str);
double s2d(string str);
void TL4000Setting(ViSession handle);
void ChangeLdCurrSetpoint(ViSession handle);
void ChangeTempSetpoint(ViSession handle);
void StartDevice(ViSession handle);
void CloseDevice(ViSession handle);

/*===========================================================================
 Functions
===========================================================================*/
int main()
{
    ViStatus    err;
    ViSession   instrHdl = VI_NULL;

    // Open session to instrument
    cout << "Opening session to '" << resourceName << "' ...\n";
    err = TL4000_init(resourceName, VI_ON, VI_ON, &instrHdl);
    if (err) ErrorExit(instrHdl, err);  // can not open session to instrument. 
    else cout << "'" << resourceName << "' is connected. \n";

    //set and display settings
    TL4000Setting(instrHdl);

    //change the current and temperature setpoint
    ChangeLdCurrSetpoint(instrHdl);
    ChangeTempSetpoint(instrHdl);

    //Start Device
    StartDevice(instrHdl);

    // Close session to instrument
    CloseDevice(instrHdl);

    return (EXIT_SUCCESS);
}


/*---------------------------------------------------------------------------
  Exit with error message
---------------------------------------------------------------------------*/
void ErrorExit(ViSession instrHdl, ViStatus err)
{
    ViChar   buf[TL4000_ERR_DESCR_BUFFER_SIZE];

    // Get error description and print out error
    TL4000_errorMessage(instrHdl, err, buf);
    cerr << "ERROR: " << buf << endl;

    // close session to instrument if open
    if (instrHdl != VI_NULL)
    {
        TL4000_close(instrHdl);
    }

    // exit program
    exit(EXIT_FAILURE);
}

/*---------------------------------------------------------------------------
  Error message
---------------------------------------------------------------------------*/
void Error(ViSession instrHdl, ViStatus err)
{
    ViChar   buf[TL4000_ERR_DESCR_BUFFER_SIZE];

    // Get error description and print out error
    TL4000_errorMessage(instrHdl, err, buf);
    cerr << "ERROR: " << buf << endl;
}

/*---------------------------------------------------------------------------
  Judge if the entered value is valid
---------------------------------------------------------------------------*/
bool IsNumber(string str)
{
    char* a = new char[str.length()];
    strcpy_s(a, str.length()+1,str.c_str());
    int i;
    for (i = 0; i < str.length(); i++)
    {
        if ((!isdigit(a[i])) && (a[i] != '.')) return false;
    }
    return true;
}

/*---------------------------------------------------------------------------
  Convert from string to double
---------------------------------------------------------------------------*/
double s2d(string str)
{
    return atof(str.c_str());
}

/*---------------------------------------------------------------------------
  Set and Display the Settings
---------------------------------------------------------------------------*/
void TL4000Setting(ViSession handle) 
{
    ViStatus    err;

    //Set parameters
    err = TL4000_setTempSensor(handle, tempSensor);
    if (err) Error(handle, err);

    err = TL4000_setLdOperatingMode(handle, ldOperatingMode);
    if (err) Error(handle, err);

    err = TL4000_setLdCurrLimit(handle, ldCurrLimit);
    if (err) Error(handle, err);

    err = TL4000_setLdPolarity(handle, ldPolarity);
    if (err) Error(handle, err);

    err = TL4000_setPdPolarity(handle, pdPolarity);
    if (err) Error(handle, err);

    //read parameters
    ViInt16 tempSensor2 = VI_NULL;
    ViInt16 ldOperatingMode2 = VI_NULL;
    ViReal64 ldCurrLimit2 = VI_NULL;
    ViInt16 ldPolarity2 = VI_NULL;
    ViInt16 pdPolarity2 = VI_NULL;
    TL4000_getTempSensor(handle, &tempSensor2);
    TL4000_getLdOperatingMode(handle, &ldOperatingMode2);
    TL4000_getLdCurrLimit(handle, 0, &ldCurrLimit2);
    TL4000_getLdPolarity(handle, &ldPolarity2);
    TL4000_getPdPolarity(handle, &pdPolarity2);

    //print paramenters
    cout << "\nThe current saved settings are:\n";
    cout << "Temperature Sensor: ";
    switch (tempSensor2)
    {
        case 0: 
            cout << "AD590 Sensor\n";
            break;
        case 1:
            cout << "Thermistor (Low Range)\n";
            break;
        case 2:
            cout << "Thermistor (High Range)\n";
            break;
        case 3:
            cout << "PT100 RTD Sensor\n";
            break;
        case 4:
            cout << "PT1000 RTD Sensor\n";
            break;
        case 5:
            cout << "LM35 Sensor\n";
            break;
        case 6:
            cout << "LM335 Sensor\n";
            break;
    }
    
    cout << "LD Operating Mode: ";
    switch (ldOperatingMode2)
    {
        case 0:
            cout << "Constant Current Mode\n";
            break;
        case 1:
            cout << "Constant Power Mode\n";
            break;
    }

    cout << "LD Current Limit: " << ldCurrLimit2 << " A\n";

    cout << "LD Polarity: ";
    switch (ldPolarity2)
    {
        case 0:
            cout << "Cathode Grounded (CG)\n";
            break;
        case 1:
            cout << "Anode Grounded (AG)\n";
            break;
    }
    
    cout << "PD Polarity: ";
    switch (pdPolarity2)
    {
        case 0:
            cout << "Cathode Grounded (CG)\n";
            break;
        case 1:
            cout << "Anode Grounded (AG)\n";
            break;
    }

    char a;
    cout << "Press 'N' to Exit the Program to Modify the Parameters. Press ANY OTHER KEY to proceed.";
    cin >> a;
    if (a == 'N')
    {
        TL4000_close(handle);

        // exit program
        exit(EXIT_FAILURE);
    }
}

/*---------------------------------------------------------------------------
  Change the LD Current Setpoint
---------------------------------------------------------------------------*/
void ChangeLdCurrSetpoint(ViSession handle)
{
    ViReal64 laserDiodeCurrSetpoint = VI_NULL;
    string laserDiodeCurrSetpoint2;
    bool success = false;
    char a;
    ViStatus err;
    ViSession instrHdl = VI_NULL;

    TL4000_getLdCurrSetpoint(handle, 0, &laserDiodeCurrSetpoint);
    cout << "\nLD Current Setpoint: " << laserDiodeCurrSetpoint << " A\n";

    while (!success) 
    {
        cout << "Change the LD Current Setpoint? (Press 'Y' or 'N') ";
        cin >> a;
        if (a == 'Y')
        {
            cout << "Enter the New Current Setpoint: ";
            cin >> laserDiodeCurrSetpoint2;
            //If the input value is valid, set this value as the current setpoint
            if (IsNumber(laserDiodeCurrSetpoint2))
            {
                err = TL4000_setLdCurrSetpoint(handle, s2d(laserDiodeCurrSetpoint2));
                if (err) Error(instrHdl, err);
                else
                {
                    TL4000_getLdCurrSetpoint(handle, 0, &laserDiodeCurrSetpoint);
                    success = true;
                }
            }
            else cout << "Invalid Number! "; 
        }
        else if (a == 'N')
        {
            success = true;
        }
    }
    cout << "LD Current Setpoint: " << laserDiodeCurrSetpoint << " A\n";
}

/*---------------------------------------------------------------------------
  Change the Temperature Setpoint
---------------------------------------------------------------------------*/
void ChangeTempSetpoint(ViSession handle)
{
    ViReal64 tempSetpoint = VI_NULL;
    string tempSetpoint2;
    bool success = false;
    char a;
    ViStatus err;
    ViSession instrHdl = VI_NULL;

    //get the temperature unit
    ViInt16 tempUnit = VI_NULL;
    string tempUnitString = "";
    TL4000_getTempUnit(handle, &tempUnit);

    TL4000_getTempSetpoint(handle, 0, &tempSetpoint);
    cout << "Temperature Setpoint: " << tempSetpoint << " ";
    switch (tempUnit)
    {
        case 0:
            cout << "Degree Celsius\n";
            tempUnitString = "Degree Celsius";
            break;
        case 1:
            cout << "Kelvin\n";
            tempUnitString = "Kelvin";
            break;
        case 2:
            cout << "Fahrenheit\n";
            tempUnitString = "Fahrenheit";
            break;
    }

    while (!success)
    {
        cout << "Change the Temperature Setpoint? (Press 'Y' or 'N') ";
        cin >> a;
        if (a == 'Y')
        {
            cout << "Enter the New Temperature Setpoint: ";
            cin >> tempSetpoint2;
            //If the input value is valid, set this value as the temperature setpoint
            if (IsNumber(tempSetpoint2))
            {
                err = TL4000_setTempSetpoint(handle, s2d(tempSetpoint2));
                if (err) Error(instrHdl, err);
                else
                {
                    TL4000_getTempSetpoint(handle, 0, &tempSetpoint);
                    success = true;
                }
            }
            else cout << "Invalid Number! ";
        }
        else if (a == 'N')
        {
            success = true;
        }
    }
    cout << "Temperature Setpoint: " << tempSetpoint << " " << tempUnitString << "\n";
}

/*---------------------------------------------------------------------------
  Turn on the TEC and the LD
---------------------------------------------------------------------------*/
void StartDevice(ViSession handle)
{
    ViStatus err;
    ViSession instrHdl = VI_NULL;
    char a;

    cout << "Turn on the TEC and the LD? (Press 'Y' or 'N')\n";
    cin >> a;
    if (a == 'Y')
    {
        err = TL4000_switchTecOutput(handle, true);
        if (err) ErrorExit(instrHdl, err);
        else
        {
            cout << "TEC is turned on.\n";
            err = TL4000_switchLdOutput(handle, true);
            if (err) ErrorExit(instrHdl, err);
            else cout << "LD is turned on.\n";
        }
    }
    else return;
}
/*---------------------------------------------------------------------------
  Turn off the TEC and the LD. Then disconnect the device.
---------------------------------------------------------------------------*/
void CloseDevice(ViSession handle) 
{
    ViStatus err;
    ViSession instrHdl = VI_NULL;
    ViBoolean statusLD = VI_NULL;
    ViBoolean statusTEC = VI_NULL;
    char a;

    TL4000_getLdOutputState(handle, &statusLD);
    TL4000_getTecOutputState(handle, &statusTEC);

    while (statusLD == true || statusTEC == true)
    {
        cout << "Press 'Y' to stop the TEC and LD output. \n";
        cin >> a;
        if (a == 'Y' )
        {
            err = TL4000_switchLdOutput(handle, false);
            if (err) ErrorExit(instrHdl, err);
            else
            {
                cout << "LD is turned off.\n";
                err = TL4000_switchTecOutput(handle, false);
                if (err) ErrorExit(instrHdl, err);
                else cout << "TEC is turned off.\n";
            }
            TL4000_getLdOutputState(handle, &statusLD);
            TL4000_getTecOutputState(handle, &statusTEC);
        }  
    }
    TL4000_close(handle);
    cout << "Program Finishes.\n\n";
}

/****************************************************************************
  End of Source file
****************************************************************************/
