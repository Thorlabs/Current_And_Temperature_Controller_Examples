using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Thorlabs.TL4000;

namespace ITC4000
{
    public class Program
    {
        string address = "USB::4883::32842::M00443953::INSTR";//change to correct serial number
        static TL4000 device;

        static void Main(string[] args)
        {
            Program ITC = new Program();
           
            //Initialize device
            try
            {
                ITC.InitializeDevice();
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Press any key to exit");
                Console.ReadKey();
                return;
            }

            //Display and modify settings
            device.setTecOperatingMode(1);
            Console.WriteLine("TEC operating mode set to constant temperature\n");
            ITC.ChangeTecCurrLimit();
            device.setTempUnit(0);//(0) °Celsius, (1) Kelvin, (2) Fahrenheit
            ITC.ChangeTemperatureSetpoint();
            device.setTempSensor(0);//(0): AD590 sensor, (1): Thermistor (low range),  (2): Thermistor (high range),  (3): PT100 RTD sensor, (4): PT1000 RTD sensor,  (5): LM35 sensor, (6): LM335 sensor
            Console.WriteLine("Temperature sensor set to AD590\n");
            
            ITC.AutoTune();
            
            ITC.ChangeLdCurrentLimit();
            ITC.ChangeLdCurrentSetpoint();
            //make sure you made all necessary settings before you turn on the laser diode with "device.setLdEnableMode(1);"

            //Check device status
            StringBuilder errormessage = new StringBuilder();
            device.errorQuery(out int Error_Number, errormessage);
            if (Error_Number != 0) Console.WriteLine(errormessage+"\n");

            //Turn on TEC output and measure temperature
            device.switchTecOutput(true);
            Console.WriteLine("\nSwitch on TEC output");
            for (int i = 0; i < 10; i++) {
                device.measTemp(out double readtemperature);
                Console.WriteLine("Temperature: " + readtemperature);
                Thread.Sleep(1000);
            }
            device.switchTecOutput(false);
            Console.WriteLine("Switch off TEC output");

            //Close connection
            Console.WriteLine("Disconnect");

            device.Dispose();

            Console.WriteLine("Press any key to exit.");
            Console.ReadKey();
            
        }
        void ChangeTecCurrLimit()
        {
            device.getTecCurrLimit(0, out double TECCurrentLimit);
            Console.WriteLine("TEC current limit [A]:" + TECCurrentLimit);
            bool success =false;
            while (! success)
            {
                Console.WriteLine("Change TEC current limit? (press 'n' or enter value)");
                string value = Console.ReadLine();
                if (value == "n")
                {
                    success = true;
                    //return;
                }
                else
                {
                    try
                    {
                        device.setTecCurrLimit(Convert.ToDouble(value));
                        success = true;

                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);

                    }
                }
            }
            device.getTecCurrLimit(0, out double TECCurrentLimit2);
            Console.WriteLine("New TEC current limit [A]:" + TECCurrentLimit2+"\n");
        }

        void ChangeTemperatureSetpoint()
        {
            device.getTempSetpoint(0, out double TempSetpoint);
            Console.WriteLine("Temperature setpoint [°C]:" + TempSetpoint);
            bool success = false;
            while (!success)
            {
                Console.WriteLine("Change temperature setpoint? (press 'n' or enter value)");
                string value = Console.ReadLine();
                if (value == "n")
                {
                    success = true;
                    return;
                }
                else
                {
                    try
                    {
                        device.setTempSetpoint(Convert.ToDouble(value));
                        success = true;

                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);

                    }
                }
            }
            device.getTempSetpoint(0, out double TempSetpoint2);
            Console.WriteLine("New temperature setpoint:" + TempSetpoint2 + "\n");
        }

        void ChangeLdCurrentLimit()
        {
            device.getLdCurrLimit(0, out double LDCurrentLimit);
            Console.WriteLine("\nLaser diode current limit [A]:" + LDCurrentLimit);
            bool success = false;
            while (!success)
            {
                Console.WriteLine("Change laser diode current limit? (press 'n' or enter value)");
                string value = Console.ReadLine();
                if (value == "n")
                {
                    success = true;
                    return;
                }
                else
                {
                    try
                    {
                        device.setLdCurrLimit(Convert.ToDouble(value));
                        success = true;
                        
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);
                        
                    }
                }
            }
            device.getLdCurrLimit(0, out double LDCurrentLimit2);
            Console.WriteLine("New laser diode current limit [A]:" + LDCurrentLimit2);
            
        }

        void ChangeLdCurrentSetpoint()
        {
            device.getLdCurrSetpoint(0, out double LDCurrentSetpoint);
            Console.WriteLine("\nLaser diode current setpoint [A]:" + LDCurrentSetpoint);
            bool success = false;
            while (!success)
            {
                Console.WriteLine("Change laser diode current setpoint? (press 'n' or enter value)");
                string value = Console.ReadLine();
                if (value == "n")
                {
                    success = true;
                    return;
                }
                else
                {
                    try
                    {
                        device.setLdCurrSetpoint(Convert.ToDouble(value));
                        success = true;

                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);

                    }
                }
            }
            device.getLdCurrLimit(0, out double LDCurrentSetpoint2);
            Console.WriteLine("New laser diode current limit [A]:" + LDCurrentSetpoint2);

        }

        void AutoTune()
        {
            Console.WriteLine("Perform PID auto tune? (y/n)");
            string value=Console.ReadLine();
            if (value == "y")
            {
                device.startAutoPid();
                device.getAutoPidState(out short state, out short phase, out short count);
                while (state == 1)
                {
                    Console.WriteLine("PID auto tune in process. Loop count:"+count);
                    Thread.Sleep(1000);
                    device.getAutoPidState(out state, out phase, out count);
                }
                if (state == 4) Console.WriteLine("PID auto tune finished successfully");
                device.getAutoPidParams(out double Pshare, out double Ishare, out double Dshare, out double Period);
                Console.WriteLine("P: " + Pshare + "  I:" + Ishare + "  D: " + Dshare + "\n");
                device.transferAutoPidParams();
            }
            else
            {
                return;
            }
        }

        void InitializeDevice()
        {
            
            device = new TL4000(address, true, false);
            device.initiate();

            
           
        }
    }
}