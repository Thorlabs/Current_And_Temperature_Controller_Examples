// Title: ITC5000. 
// Created Date: 2026 - 06 - 05
// Last modified date: 2026 - 06 - 05
// .NET Framework version: 4.8
// Thorlabs ITC5225 driver version: 1.0
// Notes: This example demonstrates how to initialize the controller, get information about the device and read settings.
// Tested with ITC5225

using System;
using System.Text;
using Thorlabs.TL5000_64.Interop;

namespace ITC5_sample
{
    internal class Program

    {
        private static TL5000 tl5000;
        static void Main(string[] args)
        {
            // Search for connected devices
            uint resCnt = 0;
            StringBuilder ModelName = new StringBuilder(256);
            StringBuilder SerialNumber = new StringBuilder(256);
            StringBuilder Manufacurer = new StringBuilder(256);
            bool deviceAvailable;
            StringBuilder Resource_Name = new StringBuilder(256);
            TL5000 searchdevice = new TL5000(new IntPtr());

            try
            {
                searchdevice.findRsrc(out resCnt);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error finding resources: {0}", ex.Message);
                return;
            }

            // Get information about the first device found
            int err = searchdevice.getRsrcInfo(resCnt - 1, ModelName, SerialNumber, Manufacurer, out deviceAvailable);
            Console.WriteLine("First device found:");
            Console.WriteLine("Model Name: {0}", ModelName);
            Console.WriteLine("Serial Number: {0}", SerialNumber);
            Console.WriteLine("Manufacturer: {0}", Manufacurer);
            Console.WriteLine("Device Available: {0}", deviceAvailable);

            err = searchdevice.getRsrcName(0, Resource_Name);
            Console.WriteLine("Resource Name: {0}", Resource_Name);
            searchdevice.Dispose();

            // Open the device
            tl5000 = new TL5000(Resource_Name.ToString(), true, false);

            // Check for errors
            int errorCode = 0;
            StringBuilder ErrorMessage = new StringBuilder(256);
            tl5000.error_query(out errorCode, ErrorMessage);
            if (errorCode != 0)
            {
                Console.WriteLine("Error: {0}", ErrorMessage);
                tl5000.Dispose();
                return;
            }

            // Get information about the slots
            StringBuilder DeviceName = new StringBuilder(256);
            StringBuilder DeviceType = new StringBuilder(256);
            StringBuilder FirmwareVersion = new StringBuilder(256);
            StringBuilder SlotSerialNumber = new StringBuilder(256);
            StringBuilder CalibrationMessage = new StringBuilder(256);
            uint Total_Slots;
            tl5000.getSlots(out Total_Slots);
            Console.WriteLine("\nNumber of slots: {0}", Total_Slots);
            for (uint i = 0; i < Total_Slots; i++)
            {
                tl5000.getSlotInfo(i, DeviceName, DeviceType, FirmwareVersion, SlotSerialNumber, CalibrationMessage);
                Console.WriteLine("Slot {0}:", i);
                Console.WriteLine("Device Name: {0}", DeviceName);
                Console.WriteLine("Device Type: {0}", DeviceType);
                Console.WriteLine("Firmware Version: {0}", FirmwareVersion);
                Console.WriteLine("Serial Number: {0}", SerialNumber);
                Console.WriteLine("Calibration Message: {0}", CalibrationMessage);
            }

            uint Slot = 2;
            uint TemperatureSensorType;
            tl5000.getTecTempSensor(Slot, out TemperatureSensorType);
            switch (TemperatureSensorType)
            {
                case 0:
                    Console.WriteLine("Temperature Sensor Type: AD590");
                    break;
                case 1:
                    Console.WriteLine("Temperature Sensor Type: Low-range thermistor");
                    break;
                case 2:
                    Console.WriteLine("Temperature Sensor Type: Mid-range thermistor");
                    break;
                case 3:
                    Console.WriteLine("Temperature Sensor Type: High-range thermistor");
                    break;
                default:
                    Console.WriteLine("Temperature Sensor Type: Unknown");
                    break;
            }

            // Get LDC operating mode
            uint ldOperatingMode;
            Slot = 1;
            tl5000.getLdcOperatingMode(Slot, out ldOperatingMode);
            switch (ldOperatingMode)
            {
                case 0:
                    Console.WriteLine("LDC Operating Mode: constant current mode");
                    break;
                case 1:
                    Console.WriteLine("LDC Operating Mode: constant power mode");
                    break;
                default:
                    Console.WriteLine("LDC Operating Mode: unknown");
                    break;
            }

            // Get LDC current limit and setpoint
            double LD_Current_Limit;
            double LD_Current_Setpoint;
            tl5000.getLdcLdCurrLimit(Slot, 0, out LD_Current_Limit);
            Console.WriteLine("LD Current Limit: {0}", LD_Current_Limit);
            tl5000.getLdcLdCurrSetpoint(Slot, 0, out LD_Current_Setpoint);
            Console.WriteLine("LD Current Setpoint: {0}", LD_Current_Setpoint);

            // Get Laser Diode Controller Laser Diode polarity
            uint ldPolarity;
            tl5000.getLdcLdPolarity(Slot, out ldPolarity);
            switch (ldPolarity)
            {
                case 0:
                    Console.WriteLine("LD Polarity: anode-grounded");
                    break;
                case 1:
                    Console.WriteLine("LD Polarity: cathode-grounded");
                    break;
                default:
                    Console.WriteLine("LD Polarity: unknown");
                    break;
            }

            tl5000.Dispose();
            Console.WriteLine("\nDevice closed.");


        }
    }
}


