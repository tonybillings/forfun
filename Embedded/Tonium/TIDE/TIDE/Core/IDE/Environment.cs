using System;
using TIDE.Code;

namespace TIDE.IDE
{
    public static class Environment
    {
        #region Public Variables
        public static string ExecPath { get; set; }
        public static Language Language { get; set; }
        public static string Filename { get; set; }
        public static string Directory { get; set; }
        public static bool Unsaved { get; set; }
        public static bool AnimationActive { get; set; }
        public static bool AutoSizeOutputWindowMenu { get; set; }
        #endregion

        #region Public Methods
        public static void SetOptions()
        {
            Options.Set("Environment.Language", Language.ToString());
            Options.Set("Environment.Directory", Directory);
            Options.Set("Environment.AutoSizeOutputWindowMenu", AutoSizeOutputWindowMenu);
        }

        public static void GetOptions()
        {
            string language = Options.Get<string>("Environment.Language");
            foreach (Language lang in Enum.GetValues(typeof(Language)))
            {
                if (lang.ToString() == language)
                    Language = lang;
            }

            Directory = Options.Get<string>("Environment.Directory");
            AutoSizeOutputWindowMenu = Options.Get<bool>("Environment.AutoSizeOutputWindowMenu");
        }
        #endregion
    }
}
