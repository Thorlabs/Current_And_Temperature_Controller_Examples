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
            tltspb = new TLTSPB("4883_33018_M00506857", false, false);
            printInternalTemp();
            printHum();
            tltspb.Dispose();
        }

        static void printInternalTemp()
        {
            double temp;
            tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel1, 0, out temp);
            Console.WriteLine("Temperature for Internal Channel: " + temp);
        }

        static void printExtTemp1()
        {
            double temp;
            tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel2, 0, out temp);
            Console.WriteLine("Temperature for Channel 2: " + temp);
        }

        static void printExtTemp2()
        {
            double temp;
            tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel3, 0, out temp);
            Console.WriteLine("Temperature for Channel 3: " + temp);
        }

        static void printHum()
        {
            double humid;
            tltspb.getHumidityData(0, out humid);
            Console.WriteLine("Humidity Value: " + humid);
        }

        static void setOffset(int offset)
        {
            tltspb.setTempSensOffset(TLTSPBConstants.TltspTemperChannel1, offset);
            System.Threading.Thread.Sleep(500);

        }

        static void printOffset()
        {
            double temperatureValue;
            tltspb.getTemperatureData(TLTSPBConstants.TltspTemperChannel1, 0, out temperatureValue);
            Console.WriteLine("Current Temp Offset: " + temperatureValue);
        }

        static void setThermExp(double r0, double t0, double beta)
        {
            tltspb.setThermistorExpParams(TLTSPBConstants.TltspTemperChannel2, r0, t0, beta);
            System.Threading.Thread.Sleep(500);
        }

        static void printThermExp()
        {
            double r0;
            double t0;
            double beta;
            tltspb.getThermistorExpParams(TLTSPBConstants.TltspTemperChannel2, 0, out r0, out t0, out beta);
            Console.WriteLine("R_0 value: " + r0 + "/ T_0 value: " + t0 + "/ Beta value: " + beta);
        }

        static void printThermRes()
        {
            double res;
            tltspb.getThermRes(TLTSPBConstants.TltspTemperChannel2, 0, out res);
            Console.WriteLine("External Thermistor Resistance: " + res);

        }


    }
}
