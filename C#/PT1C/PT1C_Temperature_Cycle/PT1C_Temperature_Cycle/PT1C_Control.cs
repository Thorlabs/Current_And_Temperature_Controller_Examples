using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Ports;

namespace PT1C_Temperature_Cycle
{
    public class PT1C_Control
    {
        SerialPort pt1cPort;
        public bool Connected 
        {
            get;
            set;
        }
        public PT1C_Control(string portName)
        {
            //Try initializing the port with the supplued port name
            try
            {
                pt1cPort = new SerialPort();
                pt1cPort.PortName = portName;
                pt1cPort.DataBits = 8;
                pt1cPort.StopBits = StopBits.One;
                pt1cPort.BaudRate = 115200;
                pt1cPort.Parity = Parity.None;
                pt1cPort.Handshake = Handshake.None;
                pt1cPort.ReadTimeout = 1000;
                pt1cPort.WriteTimeout = 500;
                pt1cPort.RtsEnable = true;
                pt1cPort.NewLine = "\r"; //Returns from the pt1c are ended with a carriage return character
                pt1cPort.Open();
                Connected = true;
            }
            catch (Exception)
            {
                Console.WriteLine("Error During Port Connection");
                Connected = false;
            }
        }

        /// <summary>
        /// Get the Hardware Info of the PT1C like Firmware Version
        /// </summary>
        /// <param name="info"></param>
        /// <returns>int representing the command status. 0 for Success, 1 for Error</returns>
        public int GetHardwareInfo(out string info)
        {
            string response = SendCommand("m?\r");
            info = "";
            if (response.Equals("WRITE_ERROR") || response.Equals("READ_ERROR"))
            {
                return 1;
            }
            else
            {
                info = response;
                return 0;
            }
        }

        /// <summary>
        /// Get the Operating Current of the TEC in the PT1C. 
        /// </summary>
        /// <param name="temp"></param>
        /// <returns>int representing the command status. 0 for Success, 1 for Error</returns>
        public int GetTecCurrent(out double tecCurrent)
        {
            string response = SendCommand("A?\r");
            tecCurrent = Double.NaN;
            if (response.Equals("WRITE_ERROR") || response.Equals("READ_ERROR"))
            {
                return 1;
            }
            else
            {
                tecCurrent = double.Parse(response);
                return 0;
            }
        }

        /// <summary>
        /// Set the Temperature Set Point for the PT1C
        /// </summary>
        /// <param name="temp"></param>
        /// <returns>int representing the command status. 0 for Success, 1 for Error</returns>
        public int SetTemperature(double temperature)
        {
            string response = SendCommand("T" + (int)(temperature*1000) +"\r");
            if (response.Equals("WRITE_ERROR") || response.Equals("READ_ERROR"))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        /// <summary>
        /// Get the Actual Temperature of the PT1C
        /// </summary>
        /// <param name="temp"></param>
        /// <returns>int representing the command status. 0 for Success, 1 for Error</returns>
        public int GetTemperature(out double temp)
        {
            string response = SendCommand("Te?\r");
            temp = Double.NaN;
            if (response.Equals("WRITE_ERROR") || response.Equals("READ_ERROR"))
            {
                return 1;
            }
            else
            {
                temp = double.Parse(response)/1000;
                return 0;
            }
        }

        /// <summary>
        /// Get the Temperature Set Point for the PT1C
        /// </summary>
        /// <param name="temp"></param>
        /// <returns>int representing the command status. 0 for Success, 1 for Error</returns>
        public int GetTemperatureSetPoint(out double temp)
        {
            string response = SendCommand("T?\r");
            temp = Double.NaN;
            if (response.Equals("WRITE_ERROR") || response.Equals("READ_ERROR"))
            {
                return 1;
            }
            else
            {
                temp = double.Parse(response)/1000;
                return 0;
            }
        }

        public void Close()
        {
            if (Connected)
            {
                pt1cPort.Close();
            }
        }

        /// <summary>
        /// Write to the port. Method waits 2 seconds for a response before throwing an error.
        /// </summary>
        /// <param name="command"></param>
        /// <returns>string containing the device response or an error string</returns>
        private string SendCommand(string command)
        {
            string response = "";
            try
            {
                pt1cPort.Write(command);
            }
            catch (Exception ex)
            {
                return "WRITE_ERROR";
            }
            long startTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
            long endTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
            while (!response.Contains(">"))
            {
                try
                {
                    response += pt1cPort.ReadExisting();
                }
                catch (Exception)
                {
                    return "READ_ERROR";
                }
                if (endTime - startTime > 2000)
                {
                    return  "READ_ERROR";
                }
                endTime = DateTimeOffset.Now.ToUnixTimeMilliseconds();
            }
            if (response.Contains(">"))
            {
                return response.Substring(0, response.IndexOf('>'));
            }
            else { return "READ_Error"; }
        }
    }
}
