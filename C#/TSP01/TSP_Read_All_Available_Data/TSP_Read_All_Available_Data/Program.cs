using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Thorlabs.TLTSP01B_64.Interop;

namespace TSP_Read_All_Available_Data
{
    class Program
    {
        private static TLTSPB tltspb = null;

        static void Main(string[] args)
        {
            //Initialize a empty TSP to allow for resource finding
            tltspb = new TLTSPB(new IntPtr(0));
            uint numDevices = 0;
            tltspb.findRsrc(out numDevices);
            if (numDevices == 0)
            {
                //Close if no devices are found
                Console.WriteLine("No Devices Found");
                return;
            }

            // Get the resource name of the first available device
            StringBuilder resourceName = new StringBuilder(256);
            tltspb.getRsrcName(0, resourceName);
            tltspb.Dispose(); //Close the reference to the empty TSP

            //reinitialize with the found resource string. The 2 booleans are for ID Query and reset. These can both be false
            tltspb = new TLTSPB(resourceName.ToString(), false, false);

            //Measurements
            //Device attributes for all "get" commands shoud be 0. 
            //The attribute represents the current, min, or max value and would only need to be used when setting parmeters

            //Measure the humidity
            double humidity;
            tltspb.getHumidityData(0, out humidity);
            Console.WriteLine("Humidity Value: " + humidity);

            //Measure the temperate using the internal sensor
            double temp;
            tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel1, 0, out temp);
            Console.WriteLine("Temperature for Internal Channel: " + temp);

            //Attempt to measure data from the external probes. If none are connected an exception will be thrown
            try
            {
                tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel2, 0, out temp);
                Console.WriteLine("Temperature for External Probe 1: " + temp);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Sensor 1 is not connected");
            }

            try
            {
                tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel3, 0, out temp);
                Console.WriteLine("Temperature for External Probe 2: " + temp);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Sensor 2 is not connected");
            }
            //Disconnect
            tltspb.Dispose();
            Console.ReadKey();
        }
    }
}
