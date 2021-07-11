using System;
using System.Text;

namespace TIDE.IDE
{
    public static class Logger
    {
        #region Events
        public delegate void DataClearedHandler();
        public static event DataClearedHandler DataCleared;
        public static event EventHandler OutputChanged;
        #endregion

        #region Private Constants
        private const string LOG_TEMPLATE = @"
=========================================================================
TIDE {TYPE} [{TIME}]
{MSG}
=========================================================================";
        #endregion

        #region Private Variables
        private static StringBuilder _data;
        private static string _output;
        #endregion

        #region Constructor
        static Logger()
        {
            Init();
        }
        #endregion

        #region Properties
        public static string Output
        {
            get
            {
                return _output;
            }
            set
            {
                if (value != _output)
                {
                    _output = value;

                    if (OutputChanged != null)
                        OutputChanged(null, EventArgs.Empty);
                }
            }
        }
        #endregion

        #region Private Methods
        private static void Init()
        {
            _data = new StringBuilder();
        }
        #endregion

        #region Public Methods
        public static void Clear()
        {
            _data.Clear();

            if (DataCleared != null)
                DataCleared();
        }

        public static void Input(string data, bool appendNewLine = true)
        {
            if (String.IsNullOrEmpty(data)) return;

            if (appendNewLine)
                _data.Append(data + System.Environment.NewLine);
            else
                _data.Append(data);

            try { Output = _data.ToString(); } catch { }
        }

        public static void Log(string type, string message)
        {
            string now = DateTime.Now.ToString("yyyy-MM-dd  hh:mm:ss");
            string entry = LOG_TEMPLATE.Replace("{TYPE}", type).Replace("{TIME}", now).Replace("{MSG}", message);
            Input(entry);
        }

        public static void LogInfo(string message)
        {
            Log("INFO", message);
        }

        public static void LogError(string message)
        {
            Log("ERROR", message);
        }
        #endregion
    }
}
