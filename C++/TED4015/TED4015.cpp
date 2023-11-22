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
//Setting about the TED4015
ViChar resourceName[256] = "USB0::0x1313::0x8048::M00463911::INSTR"; //change to correct serial number. It can be found in the "Remote Control" in Menu Screen.

//Settings about the TEC element
ViInt16 tecOperatingMode = 1;//(0): Current Source Mode, (1): Constant Temperature Mode
ViReal64 tecCurrLimit = 4;//current unit: A

//Settings about the temperature sensor
ViInt16 tempSensor = 1;//(0): AD590 sensor, (1): Thermistor (low range),  (2): Thermistor (high range),  (3): PT100 RTD sensor, (4): PT1000 RTD sensor,  (5): LM35 sensor, (6): LM335 sensor
ViInt16 thermistorCalcMethod = 0;//(0): Exponential Method;  (1): Steinhart-Hart Method
ViReal64 thermistorExp_r0 = 10000; //unit: Ohm. The R0 value of the Exponential Method parameters
ViReal64 thermistorExp_t0 = 25;//unit: degree celsius. the T0 value of the Exponential Method parameters
ViReal64 thermistorExp_beta = 3988;//the beta value of the Exponential Method parameters
ViReal64 thermistorShh_A = 1.129242 * 0.001;//the A value of the Steinhart-Hart Method parameters
ViReal64 thermistorShh_B = 2.341077 * 0.0001;//the B value of the Steinhart-Hart Method parameters
ViReal64 thermistorShh_C = 0.08765 * 0.000001;//the C value of the Steinhart-Hart Method parameters
/*===========================================================================
 Prototypes
===========================================================================*/
void ErrorExit(ViSession handle, ViStatus err);
void Error(ViSession handle, ViStatus err);
bool IsNumber(string str);
double s2d(string str);
void TL4000Setting(ViSession handle);
void ChangeTecCurrSetpoint(ViSession handle);
void ChangeTempSetpoint(ViSession handle);
void PIDAutoTune(ViSession handle);
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
    else cout << "'" << resourceName << "' is connected. \n\n";

    //set and display settings
    TL4000Setting(instrHdl);

    //change the current and temperature setpoint
    if (tecOperatingMode == 0)
        ChangeTecCurrSetpoint(instrHdl);
    else if (tecOperatingMode == 1)
        ChangeTempSetpoint(instrHdl);

    //Auto PID Tunning
    PIDAutoTune(instrHdl);

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
    strcpy_s(a, str.length() + 1, str.c_str());
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
    err = TL4000_setTecOperatingMode(handle, tecOperatingMode);
    if (err) Error(handle, err);

    err = TL4000_setTecCurrLimit(handle, tecCurrLimit);
    if (err) Error(handle, err);

    err = TL4000_setTempSensor(handle, tempSensor);
    if (err) Error(handle, err);

    if (tempSensor == 1 or tempSensor == 2) // if the thermistor is used, the parameters should be set
    {
        err = TL4000_setThermistorMethod(handle, thermistorCalcMethod);
        if (err) Error(handle, err);

        //Exponential Method
        if (thermistorCalcMethod == 0)
        {
            err = TL4000_setThermistorExpParams(handle, thermistorExp_r0, thermistorExp_t0, thermistorExp_beta);
            if (err) Error(handle, err);
        }
        else if (thermistorCalcMethod == 1)
        {
            err = TL4000_setThermistorShhParams(handle, thermistorShh_A, thermistorShh_B, thermistorShh_C);
            if (err) Error(handle, err);
        }
    }


    //read parameters
    ViInt16 tecOperatingMode2 = VI_NULL;
    ViReal64 tecCurrLimit2 = VI_NULL;
    ViInt16 tempSensor2 = VI_NULL;
    ViInt16 thermistorCalcMethod2 = VI_NULL;
    ViReal64 thermistorExp_r02 = VI_NULL;
    ViReal64 thermistorExp_t02 = VI_NULL;
    ViReal64 thermistorExp_beta2 = VI_NULL;
    ViReal64 thermistorShh_A2 = VI_NULL;
    ViReal64 thermistorShh_B2 = VI_NULL;
    ViReal64 thermistorShh_C2 = VI_NULL;

    TL4000_getTecOperatingMode(handle, &tecOperatingMode2);
    TL4000_getTecCurrLimit(handle, 0, &tecCurrLimit2);
    TL4000_getTempSensor(handle, &tempSensor2);
    
    if (tempSensor2 == 1 or tempSensor2 == 2) // if the thermistor is used, the parameters should be read
    {
        TL4000_getThermistorMethod(handle, &thermistorCalcMethod2);
        //Exponential Method
        if (thermistorCalcMethod2 == 0)
        {
            err = TL4000_getThermistorExpParams(handle, 0, &thermistorExp_r02, &thermistorExp_t02, &thermistorExp_beta2);
            if (err) Error(handle, err);
        }
        else if (thermistorCalcMethod2 == 1)
        {
            err = TL4000_getThermistorShhParams(handle, 0, &thermistorShh_A2, &thermistorShh_B2, &thermistorShh_C2);
            if (err) Error(handle, err);
        }
    }


    //print paramenters
    cout << "\nThe current saved settings are:\n";

    cout << "TEC Operating Mode: ";
    switch (tecOperatingMode2)
    {
    case 0:
        cout << "Current Source Mode\n";
        break;
    case 1:
        cout << "Constant Temperature Mode\n";
        break;
    }

    cout << "TEC Current Limit: " << tecCurrLimit2 << " A\n";

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

    if (tempSensor2 == 1 or tempSensor2 == 2) // if the thermistor is used, the parameters should be shown
    {
        //Exponential Method
        if (thermistorCalcMethod2 == 0)
        {
            cout << "Exponential Method Parameters: R0 = " << thermistorExp_r02 << ", T0 = " << thermistorExp_t02 << ", Beta = " << thermistorExp_beta2 << " \n";
        }
        //Steinhart-Hart Method
        else if (thermistorCalcMethod2 == 1)
        {
            cout << "Steinhart-Hart Method Parameters: A = " << thermistorShh_A2 << ", B = " << thermistorShh_B2 << ", C = " << thermistorShh_C2 << " \n";
        }
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
  Change the TEC Current Setpoint
---------------------------------------------------------------------------*/
void ChangeTecCurrSetpoint(ViSession handle)
{
    ViReal64 tecCurrSetpoint = VI_NULL;
    string tecCurrSetpoint2;
    bool success = false;
    char a;
    ViStatus err;
    ViSession instrHdl = VI_NULL;

    TL4000_getTecCurrSetpoint(handle, 0, &tecCurrSetpoint);
    cout << "\nTEC Current Setpoint: " << tecCurrSetpoint << " A\n";

    while (!success)
    {
        cout << "Change the TEC Current Setpoint? (Press 'Y' or 'N') ";
        cin >> a;
        if (a == 'Y')
        {
            cout << "Enter the New TEC Current Setpoint: ";
            cin >> tecCurrSetpoint2;
            //If the input value is valid, set this value as the current setpoint
            if (IsNumber(tecCurrSetpoint2))
            {
                err = TL4000_setTecCurrSetpoint(handle, s2d(tecCurrSetpoint2));
                if (err) Error(instrHdl, err);
                else
                {
                    TL4000_getTecCurrSetpoint(handle, 0, &tecCurrSetpoint);
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
    cout << "TEC Current Setpoint: " << tecCurrSetpoint << " A\n";
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
  PID Auto Tunning
---------------------------------------------------------------------------*/
void PIDAutoTune(ViSession handle)
{
    ViStatus err;
    ViSession instrHdl = VI_NULL;
    ViInt16 state = 0;
    char a;

    cout << "Start Auto PID Tunning? (Press 'Y' or 'N')\n";
    cin >> a;
    if (a == 'Y')
    {
        err = TL4000_startAutoPid(handle);
        if (err)
        {
            ErrorExit(instrHdl, err);
            return;
        }
        //Judge if the Auto PID Tunning is started
        //status is 0 : Never run for this configuration
        //status is 1 : Auto - PID is currently running
        //status is 2 : Canceled by user
        //status is 3 : Auto - PID procedure failed
        //status is 4 : Auto - PID finished successfully
        err = TL4000_getAutoPidState(handle, &state, VI_NULL, VI_NULL);
        if (state == 1)
        {
            cout << "The Auto PID Tunning is started, please wait...\n";
            while (state != 4)
            {
                err = TL4000_getAutoPidState(handle, &state, VI_NULL, VI_NULL);
                if (state == 3)
                {
                    cout << "The Auto PID Tunning fails.\n";
                    return;
                }
            }
        }
        cout << "The Auto PID Tunning succeeds.\n";
        err = TL4000_transferAutoPidParams(handle);
        if (err) Error(instrHdl, err);
    }
}

/*---------------------------------------------------------------------------
  Turn on the TEC
---------------------------------------------------------------------------*/
void StartDevice(ViSession handle)
{
    ViStatus err;
    ViSession instrHdl = VI_NULL;
    char a;

    cout << "Turn on the TEC? (Press 'Y' or 'N')\n";
    cin >> a;
    if (a == 'Y')
    {
        err = TL4000_switchTecOutput(handle, true);
        if (err) ErrorExit(instrHdl, err);
        else
        {
            cout << "TEC is turned on.\n";
        }
    }
    else return;
}
/*---------------------------------------------------------------------------
  Turn off the TEC. Then disconnect the device.
---------------------------------------------------------------------------*/
void CloseDevice(ViSession handle)
{
    ViStatus err;
    ViSession instrHdl = VI_NULL;
    ViBoolean statusTEC = VI_NULL;
    char a;

    TL4000_getTecOutputState(handle, &statusTEC);

    while (statusTEC == true)
    {
        cout << "Press 'Y' to stop the TEC output. \n";
        cin >> a;
        if (a == 'Y')
        {
            err = TL4000_switchTecOutput(handle, false);
            if (err) ErrorExit(instrHdl, err);
            else cout << "TEC is turned off.\n";
            TL4000_getTecOutputState(handle, &statusTEC);
        }
    }
    TL4000_close(handle);
    cout << "Program Finishes.\n\n";
}

/****************************************************************************
  End of Source file
****************************************************************************/
