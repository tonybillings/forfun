using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Threading;
using TIDE.IDE;

namespace TIDE.Code
{
    public static class Uploader
    {
        #region DLL Imports
        [DllImport("dll\\CSerial", CallingConvention = CallingConvention.ThisCall)]
        private static extern bool Init(uint port);

        [DllImport("dll\\CSerial", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Write(byte[] data, uint length);

        [DllImport("dll\\CSerial", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        private static extern bool ReadData(uint bufferSize, out IntPtr buffer, ref ulong bytesRead);

        [DllImport("dll\\CSerial", CallingConvention = CallingConvention.ThisCall)]
        private static extern void ToggleDtr(uint count);

        [DllImport("dll\\CSerial")]
        private static extern void Close();

        [DllImport("dll\\CSerial")]
        private static extern bool IsConnected();
        #endregion

        #region Enums
        private enum CommandBytes : byte { RAM = (byte)'R', Flash = (byte)'F', FlashAndVerify = (byte)'V', Load = (byte)'L', Erase = (byte)'E' }
        #endregion

        #region Private Variables
        private static bool _isRunning;
        private static bool _storeInFlash;
        private static byte[] _binFile;
        #endregion

        #region Properties
        public static string SerialPortName { get; set; }
        public static bool IsRunning { get { return _isRunning; } }
        #endregion

        #region Public Methods
        public static void Upload(bool storeInFlash)
        {
            if (_isRunning) return;

            _isRunning = true;
            _storeInFlash = storeInFlash;

            Logger.Clear();

            if (!VerifySerialPortName()) return;
            if (!ConnectToDevice()) return;
            if (!GetBinFile()) { Close(); return; }

            BeginOutputMessage();

            StartComm();
        }

        public static void SetOptions()
        {
            Options.Set("Uploader.SerialPortName", SerialPortName);
        }

        public static void GetOptions()
        {
            SerialPortName = Options.Get<string>("Uploader.SerialPortName");
        }
        #endregion

        #region Private Methods
        private static bool VerifySerialPortName()
        {
            if (String.IsNullOrEmpty(SerialPortName))
            {
                Logger.LogError("Serial port must be configured first.");
                _isRunning = false;
                return false;
            }

            return true;
        }

        private static bool ConnectToDevice()
        {
            try
            {
                Init(Convert.ToUInt32(SerialPortName.Substring(3)));

                if (!IsConnected())
                {
                    Logger.LogError("A connection to the device on " + SerialPortName + " could not be established.");
                    _isRunning = false;
                    return false;
                }
            }
            catch
            {
                Logger.LogError("A connection to the device on " + SerialPortName + " could not be established.");
                _isRunning = false;
                return false;
            }

            return true;
        }

        private static bool GetBinFile()
        {
            string execPath = Assembly.GetExecutingAssembly().Location.Substring(0, Assembly.GetExecutingAssembly().Location.LastIndexOf("\\"));
            string binFilePath = execPath + "\\Temp\\planAhead\\FPGA_CPU\\FPGA_CPU.runs\\impl_1\\top_module.bin";

            if (File.Exists(binFilePath))
            {
                try
                {
                    _binFile = File.ReadAllBytes(binFilePath);
                    return true;
                }
                catch
                {
                    Logger.LogError("Could not read bin file:  " + binFilePath);
                    _isRunning = false;
                    return false;
                }
            }
            else
            {
                Logger.LogError("Could not find bin file:  " + binFilePath);
                _isRunning = false;
                return false;
            }
        }

        private static void BeginOutputMessage()
        {
            string begin = @"
=========================================================================
Uploading bin to " + ((_storeInFlash) ? "flash" : "RAM") + @"...

0%                       50%                      100%
|-------------------------|------------------------|
 ";
            Logger.Input(begin, false);
        }

        private static void EndOutputMessage()
        {
            string end = @"

Bin file uploaded successfully.
=========================================================================";
            Logger.Input(end);
        }

        private static void StartComm()
        {
            Thread t = new Thread(new ThreadStart(SerialComm));
            t.Priority = ThreadPriority.Highest;
            t.IsBackground = true;
            t.Start();
        }

        private static void SerialComm()
        {
            if (!SendDtrSignal()) { Finish(); return; }
            if (!SendCommand(_storeInFlash ? CommandBytes.Flash : CommandBytes.RAM)) { Finish(); return; }
            if (!AwaitResponse('R')) { Finish(); return; }
            if (!SendBinFileSize()) { Finish(); return; }
            if (!AwaitResponse('O')) { Finish(); return; }
            if (!SendBinFile()) { Finish(); return; }
            if (!AwaitResponse('D')) { Finish(); return; }

            if (_storeInFlash)
            {
                if (!SendCommand(CommandBytes.Load)) { Finish(); return; }
                if (!AwaitResponse('D')) { Finish(); return; }
            }

            EndOutputMessage();
            Finish();
        }

        private static bool SendDtrSignal()
        {
            try
            {
                ToggleDtr(6);
            }
            catch
            {
                Logger.LogError("Could not send DTR signal.");
                _isRunning = false;
                return false;
            }

            return true;
        }

        private static bool SendCommand(CommandBytes cmd)
        {
            try
            {
                Write(new byte[] { (byte)cmd }, 1);
            }
            catch
            {
                Logger.LogError("Could not send command to device.");
                _isRunning = false;
                return false;
            }

            return true;
        }

        private static bool AwaitResponse(char responseChar)
        {
            DateTime startTime = DateTime.Now;
            TimeSpan waitThreshold = TimeSpan.FromSeconds(1);
            uint bufferSize = 1;
            IntPtr buffer = IntPtr.Zero;
            ulong bytesRead = 0;
            Byte[] byteArray = new Byte[bufferSize];

            while (IsConnected())
            {
                if (ReadData(bufferSize, out buffer, ref bytesRead))
                {
                    Marshal.Copy(buffer, byteArray, 0, (int)bufferSize);
                    if (byteArray[0] == responseChar)
                        return true;
                }

                if (DateTime.Now - startTime > waitThreshold)
                {
                    Logger.LogError("The device did not respond.");
                    _isRunning = false;
                    return false;
                }
            }

            return false;
        }

        private static bool SendBinFileSize()
        {
            try
            {
                byte[] fileSize = BitConverter.GetBytes(_binFile.Length);
                Write(fileSize, 4);
                return true;
            }
            catch
            {
                Logger.LogError("Could not send bin file size.");
                _isRunning = false;
                return false;
            }
        }

        private static bool SendBinFile()
        {
            try
            {
                int chunks = 50;
                int chunkSize = (int)Math.Floor((double)_binFile.Length / (double)chunks);
                int remainder = _binFile.Length % chunks;
                int loopCount = chunks + ((remainder == 0) ? 0 : 1);
                string square = "■";

                for (int i = 0; i < loopCount; i++)
                {
                    if (i == chunks)
                    {
                        byte[] buffer = new byte[remainder];
                        Array.Copy(_binFile, i * chunkSize, buffer, 0, remainder);
                        Write(buffer, (uint)remainder);
                    }
                    else
                    {
                        byte[] buffer = new byte[chunkSize];
                        Array.Copy(_binFile, i * chunkSize, buffer, 0, chunkSize);
                        Write(buffer, (uint)chunkSize);
                        Logger.Input(square, false);
                    }
                }
            }
            catch
            {
                Logger.LogError("Could not upload bin file.");
                _isRunning = false;
                return false;
            }

            return true;
        }

        private static void Finish()
        {
            if (IsConnected()) Close();
            _isRunning = false;
        }
        #endregion

    }
}
