using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;
using System.Xml;

namespace TIDE.IDE
{
    public static class Options
    {
        #region Private Classes
        [DataContract]
        private class Data
        {
            [DataMember]
            public Dictionary<string, object> Dict { get; set; }

            public Data()
            {
                Dict = new Dictionary<string, object>();
            }
        }
        #endregion

        #region Private Variables
        private static Data _data;
        #endregion

        #region Constructor
        static Options()
        {
            _data = new Data();
        }
        #endregion

        #region Public Methods
        public static bool Save(string fileName)
        {
            if (String.IsNullOrEmpty(fileName)) return false;

            try
            {
                DataContractSerializer serializer = new DataContractSerializer(typeof(Data));

                using (StreamWriter sw = new StreamWriter(fileName))
                {
                    using (XmlTextWriter xw = new XmlTextWriter(sw))
                    {
                        xw.Formatting = Formatting.Indented;
                        serializer.WriteObject(xw, _data);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Concat("Could not save options.\r\n\r\nException:  ", ex.Message), "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            return true;
        }

        public static bool Load(string fileName)
        {
            if (String.IsNullOrEmpty(fileName)) return false;
            if (!File.Exists(fileName)) return false;

            try
            {
                DataContractSerializer serializer = new DataContractSerializer(typeof(Data));

                using (StreamReader sr = new StreamReader(fileName))
                {
                    using (XmlTextReader xr = new XmlTextReader(sr))
                    {
                        _data = (Data)serializer.ReadObject(xr);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Concat("Could not load options.\r\n\r\nException:  ", ex.Message), "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            return true;
        }

        public static T Get<T>(string key)
        {
            object result;

            if (_data.Dict.TryGetValue(key, out result))
                return (T)result;

            return default(T);
        }

        public static void Set(string key, object value)
        {
            object result;

            if (_data.Dict.TryGetValue(key, out result))
                _data.Dict.Remove(key);

            _data.Dict.Add(key, value);
        }
        #endregion
    }

}
