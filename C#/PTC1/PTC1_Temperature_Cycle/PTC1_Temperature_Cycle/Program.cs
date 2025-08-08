// Title: PTC1 Temperature Cycle in C#. 
// Created Date: 2023 - 04 - 11
// Last modified date: 2023 - 04 - 11
// .NET version: 4.7.2
// Notes: This example demonstrates how to initialize the PTC1 
//and set up an acquisition thread to cycle the temperature between two user-supplied values. 

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;

namespace PT1C_Temperature_Cycle
{
    internal class Program
    {
        static Thread collectionThread;
        static bool running = false;
        static string portName = "";
        static double upperTemp = 0;
        static double lowerTemp = 0;
        static PTC1_Control pt;
        static bool cooling = false;
        static StreamWriter file;

        static void Main(string[] args)
        {
            Console.WriteLine("Enter the port name for the PTC1 (e.g. COM3)....");
            portName = Console.ReadLine().Trim();

            //Initialize the PT1C
            pt = new PTC1_Control(portName);
            if (!pt.Connected)
            {
                Console.WriteLine("Issue with connecting to PTC1... Closing");
                Console.ReadKey();
                return;
            }

            Console.WriteLine("Hardware Connected Successfully");
            Console.WriteLine("");

            //Get temperature set points from the command line
            string tmp = "";
            Console.WriteLine("Enter the upper temperature limit as a decimal");
            tmp = Console.ReadLine().Trim();
            try
            {
                upperTemp = Double.Parse(tmp);
            }
            catch
            {
                Console.WriteLine("Issue with parsing response... Closing");
                Console.ReadKey();
                return;
            }

            Console.WriteLine("Enter the lower temperature limit");
            tmp = Console.ReadLine().Trim();
            try
            {
                lowerTemp = Double.Parse(tmp);
            }
            catch
            {
                Console.WriteLine("Issue with parsing responsse... Closing");
                Console.ReadKey();
                return;
            }

            //Create a file if it does not exist or overwrite. File is saved in the root project directory
            File.Create("data.txt").Close();
            file = new StreamWriter("data.txt");

            //Start a new thread to handle acqiusition of temperature data
            collectionThread = new Thread(Run);
            running = true;
            collectionThread.Start();

            //Run until the user types stop into the console
            Console.WriteLine("Type stop to end acquisition");
            string stopCheck = "";
            while (!stopCheck.Equals("stop"))
            {
                stopCheck = Console.ReadLine().Trim();
            }

            Console.WriteLine("Closing Application...");
            running = false;
            Thread.Sleep(5000);
            collectionThread.Join();

            file.Close();
            pt.Close();
            Console.WriteLine("Application Closed");

            Console.ReadKey();
        }

        static void Run()
        {
            pt.SetTemperature(upperTemp);
            Thread.Sleep(500);
            double curTemp;
            double setTemp;
            double temperatureWindow = .5;
            cooling = false;
            

            pt.GetTemperature(out curTemp);
            pt.GetTemperatureSetPoint(out setTemp);
            while(running)
            {
                pt.GetTemperature(out curTemp);
                if (curTemp < setTemp + temperatureWindow/2 && curTemp > setTemp - temperatureWindow/2)
                {
                    //change setpoint
                    if (cooling)
                    {
                        //temp is low. now go high
                        pt.SetTemperature(upperTemp);
                        cooling = false;
                    }
                    else 
                    {
                        //temp is high. now go low
                        pt.SetTemperature(lowerTemp);
                        cooling = true;
                    }
                    Thread.Sleep(1000);
                    pt.GetTemperatureSetPoint(out setTemp);
                }
                file.WriteLine(curTemp + ", " + DateTime.Now.ToString());
                Thread.Sleep(1000);
            }
        }
    }
}
