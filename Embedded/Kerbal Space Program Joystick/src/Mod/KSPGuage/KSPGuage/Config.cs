using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace KSPGuage
{
    public static class Config
    {
        #region Private Variables
        private static Dictionary<string, string> _settings;
        #endregion

        #region Properties
        public static int Port
        {
            get
            {
                return GetInt("port");
            }
        }
        #endregion

        #region Private Methods
        private static int GetInt(string key)
        {
            if (_settings == null)
                return -1;

            string value = null;
            _settings.TryGetValue(key, out value);

            if (!String.IsNullOrEmpty(value))
            {
                int returnValue = -1;
                Int32.TryParse(value, out returnValue);
                return returnValue;
            }
            else
            {
                return -1;
            }
        }
        #endregion

        #region Public Methods
        public static void Load()
        {
            _settings = new Dictionary<string, string>();

            using (StreamReader sr = new StreamReader("KSPGuage.cfg"))
            {
                while (!sr.EndOfStream)
                {
                    string line = sr.ReadLine().Trim();

                    if (!String.IsNullOrEmpty(line) && !line.Contains("[") && !line.Contains("]") && line.Contains("="))
                    {
                        string[] keyValue = line.Split("=".ToCharArray());

                        if (keyValue.Length == 2)
                        {
                            string key = keyValue[0].ToLower().Trim();
                            string value = keyValue[1].Trim();

                            if (key.Length > 0 && value.Length > 0)
                                _settings.Add(key, value);
                        }
                    }
                }
            }
        }
        #endregion

    }
}
