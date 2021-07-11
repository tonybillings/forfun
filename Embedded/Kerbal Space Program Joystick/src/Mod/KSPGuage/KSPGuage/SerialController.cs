using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using UnityEngine;

namespace KSPGuage
{
    [KSPAddon(KSPAddon.Startup.MainMenu, true)]
    public class SerialController : MonoBehaviour
    {
        #region DLL Imports
        [DllImport("KSPSerial")]
        public static extern bool Init(int port);

        [DllImport("KSPSerial")]
        public static extern void Write(byte[] data, uint length);

        [DllImport("KSPSerial")]
        public static extern void Close();

        [DllImport("KSPSerial")]
        public static extern void IsConnected();
        #endregion

        #region Public Methods
        public static void Write(byte[] data)
        {
            Write(data, (uint)data.Length);
        }
        #endregion

        #region Private Methods
        private void Start()
        {
            print("KSP Guage: Starting...");

            try
            {
                Config.Load();
            }
            catch
            {
                print("KSP Guage: Config file could not be loaded");
                return;
            }

            bool isConnected = Init(Config.Port);

            if (isConnected)
                print("KSP Guage: Serial initialized on port " + Config.Port);
            else
                print("KSP Guage: Serial failed to initialize on port " + Config.Port);

            if (isConnected)
                Write(new ResetMessage().GetBytes());
        }
        #endregion
    }
}
