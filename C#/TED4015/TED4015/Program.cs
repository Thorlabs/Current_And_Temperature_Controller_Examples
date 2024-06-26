// Title: TED4015 in C#. 
// Created Date: 2023 - 04 - 12
// Last modified date: 2023 - 04 - 12
// .NET version: 4.7.2
// Thorlabs DLL version: 3.1.0.0
// Notes: The example connects to a Thorlabs TED4015 controller, makes settings for the elements and displays values. 


using System;
using System.Linq;
using System.Threading;
using Thorlabs.TL4000;
using System.Windows.Forms;

namespace program
{
    public class program
    {
        //From the Main menu, select the Remote Control, then enter the address string here
        string addressString = "USB::4883::32842::M00262781::INSTR";
        static TL4000 device;
        static void Main(string[] args)
        {
            program TED4015 = new program();
            
            try
            {
                //initialize the device
                TED4015.DeviceInitialize();   
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Press ANY KEY to exit.");
                Console.ReadKey();
                return;
            }
            
            try
            {
                //display the current saved parameters
                TED4015.ParameterDisplay();
                //modify the parameters if needed, and then display the modified parameters
                Console.WriteLine("Enter <Y> to modify the parameters. Enter ANY OTHER characters to skip the modification.");
                while (Console.ReadLine() == "Y")
                {
                    Console.WriteLine("Enter the number (1 ~ 5) to change the parameters settings. " +
                        "If multiple parameters need to be modified, please seperate the numbers with commas. (e.g. 1,2)\n" +
                            "1. TEC Current limit\n" +
                            "2. TEC Operating Mode\n" +
                            "3. TEC Temperature Setpoint\n" +
                            "4. TEC Current Setpoint\n" +
                            "5. PID parameters");
                    TED4015.ParameterModify(Console.ReadLine().Split(','));
                    Thread.Sleep(500);
                    TED4015.ParameterDisplay();
                    Console.WriteLine("Enter <Y> to modify parameters again. Enter ANY OTHER characters to stop modifying.");
                }
                //start the device
                Console.WriteLine("Enter <start> to activate the TEC output.");
                TED4015.StartDevice(Console.ReadLine());
                //stop the device. The trigger condition of the command is that <Ctrl> key is pressed.
                TED4015.StopDevice();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            Console.WriteLine("Press ANY KEY to exit.");
            device.Dispose();
            Console.ReadKey();
        }

        void DeviceInitialize()
        {
            device = new TL4000(addressString, true, false);
            device.initiate();
            Console.WriteLine($"{addressString} is initialized.");
            //close the TEC output
            device.switchTecOutput(false);
            //set temperature unit: 0--celcius, 1--Kelvin, 2--Fahrenheit
            device.setTempUnit(0);
        }

        void ParameterDisplay()
        {
            //get the parameters
            device.getTecCurrLimit(0, out double TecCurrentLimit);
            device.getTempSetpoint(0, out double TempSetpoint);
            device.getTecCurrSetpoint(0, out double TecCurrentSetpoint);
            device.getTecOperatingMode(out Int16 TecOperatingModeIndex);
            device.getPidParams(0, out double PShare, out double IShare, out double DShare, out double PidPeriod);

            string TecOperatingMode;
            if (TecOperatingModeIndex == 1)
                TecOperatingMode = "Constant Temperature Mode";
            else TecOperatingMode = "Constant Current Mode";

            //print the parameters
            Console.WriteLine("TEC Current limit: {0} A", TecCurrentLimit);
            Console.WriteLine("TEC Operating Mode: " + TecOperatingMode);
            if (TecOperatingModeIndex == 1)
                Console.WriteLine("Temperature Setpoint: {0} degrees", TempSetpoint);
            else Console.WriteLine("TEC Current Setpoint: {0} A", TecCurrentSetpoint);
            Console.WriteLine($"Proportional: {PShare} A/K, Integral: {IShare} As/K, Derivative: {DShare} A/Ks, Period: {PidPeriod} s");
        }

        void ParameterModify(string[] Index)
        {
            //Check if the input format is correct. The input characters should be integers and should be in the range of [1,5]. 
            bool IndexValidTrue = Index.All(a => {
                int b = a.Length;
                return (b == 1 && String.Compare(a, "1") >= 0 && String.Compare(a, "5") <= 0);
            });

            while (!IndexValidTrue)
            {
                Console.WriteLine("Invalid input! Please only enter 1 to 5 numbers and seperate them with commas. (e.g. 1,2).");
                Index = Console.ReadLine().Split(',');
                IndexValidTrue = Index.All(a => {
                    int b = a.Length;
                    return (b == 1 && String.Compare(a, "1") >= 0 && String.Compare(a, "5") <= 0);
                });
            }

            // modify the corresponding prameters acording the input numbers
            foreach (string index in Index)
            {
                if (index == "1")
                {
                    Console.WriteLine("Please enter the TEC Current Limit (A): ");
                    device.setTecCurrLimit(Convert.ToDouble(Console.ReadLine()));
                }
                else if (index == "2")
                {
                    Console.WriteLine("Please enter the number to choose the TEC Operating Mode: \n" +
                     "0. Constant Current Mode\n" +
                     "1. Constant Temperature Mode");
                    device.setTecOperatingMode(Convert.ToInt16(Console.ReadLine()));
                }
                else if (index == "3")
                {
                    Console.WriteLine("Please enter the TEC Temperature Setpoint (degrees celcius): ");
                    device.setTempSetpoint(Convert.ToDouble(Console.ReadLine()));
                }
                else if (index == "4")
                {
                    Console.WriteLine("Please enter the TEC Current Setpoint (A): ");
                    device.setTecCurrSetpoint(Convert.ToDouble(Console.ReadLine()));
                }
                else if (index == "5")
                {
                    Console.WriteLine("Please enter the number to choose the PID parameters setting method: \n" +
                        "0. manual setting\n" +
                        "1. auto setting");
                    string PidSettingIndex = Console.ReadLine();

                    while (PidSettingIndex != "0" && PidSettingIndex != "1")
                    {
                        Console.WriteLine("Invalid input! Please only enter 0 or 1.");
                        PidSettingIndex = Console.ReadLine();
                    }

                    if (PidSettingIndex == "0")
                    {
                        Console.WriteLine("Please enter the Propotional, Integral, Derivative and Period seperated by commas. (e.g. 15,5,10,5)");
                        string[] Pidparas = Console.ReadLine().Split(',');
                        device.setPidParams(Convert.ToDouble(Pidparas[0]), Convert.ToDouble(Pidparas[1]), Convert.ToDouble(Pidparas[2]), Convert.ToDouble(Pidparas[3]));
                    }
                    else
                    {
                        device.startAutoPid();
                        device.getAutoPidState(out short state, out short phase, out short count);
                        //state =0 : Never run for this configuration
                        //state =1 : Auto - PID is currently running
                        //state =2 : Canceled by user
                        //state =3 : Auto - PID procedure failed
                        //state =4 : Auto - PID finished successfully
                        while (state == 1)
                        {
                            Console.Write("\rAuto setting PID parameters now. Please wait  ");
                            Thread.Sleep(1000);
                            Console.Write("\rAuto setting PID parameters now. Please wait.  ");
                            Thread.Sleep(1000);
                            Console.Write("\rAuto setting PID parameters now. Please wait.. ");
                            Thread.Sleep(1000);
                            Console.Write("\rAuto setting PID parameters now. Please wait...");
                            Thread.Sleep(1000);
                            device.getAutoPidState(out state, out phase, out count);
                        }
                        if (state == 4)
                        {
                            Console.WriteLine("Auto-PID procedure succeeds.");
                            device.transferAutoPidParams();
                        }
                        else if (state == 3) Console.WriteLine("Auto-PID procedure fails.");
                    }
                }
            }
        }

        void StartDevice(string ifStart)
        {
            if (ifStart == "start" || ifStart == "Start")
            {
                //switch on the TEC output
                device.switchTecOutput(true);
                Thread.Sleep(1000);
                //Get the TEC output state
                device.getTecOutputState(out bool TecState);
                if (TecState == true)
                {
                    Console.WriteLine("TEC output is activated. Press <Ctrl> key to stop the TEC output.");

                    //Two threads are created. One is used to start real-time temperature or current display, one is used to stop the real-time display.
                    Thread threadFetchValue = new Thread(FetchValue);
                    Thread threadStopThread = new Thread(() => { AbortThread(threadFetchValue); });
                    threadFetchValue.IsBackground = true;
                    threadStopThread.IsBackground = true;
                    threadFetchValue.Start();
                    threadStopThread.Start();

                    //The program won't proceed until the "threadFetchValue" Thread stops.
                    threadStopThread.Join();
                }
                else Console.WriteLine("TEC output fails to start. Program finishes.");
            }
            else Console.WriteLine("Invalid input. Program finishes.");
        }

        void FetchValue()
        {
            //Get the TEC operating mode.
            //If it works in constant temperature mode, the real-time temperature will display. 
            //If it works in constant current mode, the real-time TEC current will display.
            device.getTecOperatingMode(out Int16 TecOperatingModeIndex);
            while (true)
            {
                if (TecOperatingModeIndex == 1)
                {
                    device.measTemp(out double temperature);
                    Console.Write("\rThe measured TEC temperature is {0} degrees celcius.", temperature);
                }
                else
                {
                    device.measTecCurr(out double tecCurrent);
                    Console.Write("\rThe measured TEC current is {0} A.", tecCurrent);
                }
                Thread.Sleep(1000);
            }
        }

        void AbortThread(Thread thread)
        {
            while (true)
            {
                //The Thread will stop if the <Ctrl> key is pressed.
                if ((Control.ModifierKeys & Keys.Control) == Keys.Control)
                {
                    thread.Abort();
                    break;
                }
            }
        }
        void StopDevice()
        {
            device.switchTecOutput(false);
            Thread.Sleep(500);
            device.getTecOutputState(out bool TecState);
            if (TecState == false)
            {
                Console.WriteLine("TEC output is stoped.");
            }
            else Console.WriteLine("Fail to stop the TEC output.");
        }
    }
}
