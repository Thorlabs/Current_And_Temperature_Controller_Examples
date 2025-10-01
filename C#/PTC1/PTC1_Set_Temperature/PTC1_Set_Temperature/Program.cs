// Title: PTC1 Set Temperature in C#. 
// Created Date: 2023 - 04 - 11
// Last modified date: 2023 - 04 - 11
// .NET version: 4.7.2
// Notes: The example demonstrates how to initialize and query the PTC1 for values. 
//The temperature setpoint is set and the current temperature is then printed.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;

namespace PT1C_Set_Temperature
{
    class Program
    {
        static string portName = "";
        static PTC1_Control pt;

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

            //Set the temperature
            pt.SetTemperature(25);
            Thread.Sleep(500);

            //Get the current temperature
            double curTemp;
            pt.GetTemperature(out curTemp);
            Console.WriteLine("Current Temperature: {0}", curTemp);

            pt.Close();
            Console.WriteLine("Application Closed");
            Console.ReadKey();
        }
    }
}
