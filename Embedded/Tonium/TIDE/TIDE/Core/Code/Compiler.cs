using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Threading;
using TIDE.Code.Parsing;
using TIDE.IDE;
using TIDE.Util;

namespace TIDE.Code
{
    #region Enums
    public enum Language { xT50, Tranquil };
    #endregion

    public static class Compiler
    {
        #region Events
        public delegate void SynthesizeProcessCompletedHandler(bool wasSuccessful);
        public static event SynthesizeProcessCompletedHandler SynthesizeProcessCompleted;
        #endregion

        #region Constants
        private const string COMMAND = "{ISE_DIR}\\PlanAhead\\bin\\planAhead.bat|-nojournal -nolog -mode batch -source \"{PROJ_DIR}\\project.tcl\"";
        #endregion

        #region Private Variables
        private static string _iseDir;
        private static string _execPath;
        private static bool _isRunning;
        private static Process _proc;
        private static Thread _thread;
        private static DateTime _startTime;
        #endregion

        #region Properties
        public static string IseDirectory
        {
            get
            {
                return _iseDir;
            }
            set
            {
                if (!String.IsNullOrEmpty(value))
                {
                    _iseDir = value;
                    if (_iseDir.EndsWith("\\")) _iseDir = _iseDir.TrimEnd("\\".ToCharArray());
                }
            }
        }
        public static bool IsRunning { get { return _isRunning; } }
        #endregion

        #region Constructor
        static Compiler()
        {
            _iseDir = @"C:\Xilinx\14.7\ISE_DS";
            _execPath = Assembly.GetExecutingAssembly().Location.Substring(0, Assembly.GetExecutingAssembly().Location.LastIndexOf("\\"));
            _isRunning = false;
        }
        #endregion

        #region Public Methods
        public static bool Validate(Language sourceType, string sourceCode)
        {
            if (_isRunning) return false;

            if (String.IsNullOrEmpty(sourceCode)) return false;

            Logger.Clear();

            _isRunning = true;

            if (!RunSteps(sourceType, sourceCode, true))
            {
                _isRunning = false;
                return false;
            }

            _isRunning = false;
            return true;
        }

        public static bool Exec(Language sourceType, string sourceCode)
        {
            if (_isRunning) return false;

            if (String.IsNullOrEmpty(sourceCode)) return false;

            Logger.Clear();

            _isRunning = true;

            if (!RunSteps(sourceType, sourceCode))
            {
                _isRunning = false;
                return false;
            }

            return true;
        }

        public static void SetOptions()
        {
            Options.Set("Compiler.IseDirectory", IseDirectory);
        }

        public static void GetOptions()
        {
            IseDirectory = Options.Get<string>("Compiler.IseDirectory");
        }
        #endregion

        #region Private Methods
        private static bool RunSteps(Language sourceType, string sourceCode, bool validateOnly = false)
        {
            _startTime = DateTime.Now;

            Logger.LogInfo(String.Concat("Compiling ", sourceType.ToString(), " to Verilog..."));
            VerilogTemplateValue[] templateValues;
            if (!CompileCode(sourceType, sourceCode, out templateValues)) return false;

            if (validateOnly)
            {
                Logger.LogInfo("Code validation complete.  No errors found.");
                return true;
            }

            Logger.LogInfo("Setting up project files...");
            if (!CopyProject()) return false;
            if (!UpdateProjectTclFile()) return false;
            if (!UpdateProjectFiles(templateValues)) return false;

            Logger.LogInfo("Synthesizing design and creating bin file...");
            if (!Synthesize()) return false;

            return true;
        }

        private static bool CompileCode(Language sourceType, string sourceCode, out VerilogTemplateValue[] templateValues)
        {
            templateValues = null;

            switch (sourceType)
            {
                case Language.xT50:
                    templateValues = CompilexT50(sourceCode);
                    break;
                case Language.Tranquil:
                    templateValues = CompileTranquil(sourceCode);
                    break;
            }

            return templateValues != null;
        }

        private static VerilogTemplateValue[] CompilexT50(string sourceCode)
        {
            try
            {
                CodeTree<xT50Node> ct = new CodeTree<xT50Node>(sourceCode);
                return xT50Converter.ToVerilogTemplateValues(ct.RootNode);
            }
            catch (SyntaxException ex)
            {
                Logger.LogError(String.Concat("Line ", ex.LineNumber.ToString(), ":  ", ex.Message));
            }
            catch (Exception ex)
            {
                Logger.LogError(String.Concat("Could not compile.\r\n\r\nException:  ", ex.Message));
            }

            return null;
        }

        private static VerilogTemplateValue[] CompileTranquil(string sourceCode)
        {
            return null;
        }

        private static bool CopyProject()
        {
            try
            {
                string temp = String.Concat(_execPath, "\\Temp");
                if (Directory.Exists(temp)) FileManager.DeleteDirectory(temp);
                Directory.CreateDirectory(temp);
                FileManager.DirectoryCopy(String.Concat(_execPath, "\\FPGA"), temp, true);
            }
            catch (Exception ex)
            {
                Logger.LogError(String.Concat("Could not copy project.\r\n\r\nException:  ", ex.Message));
                return false;
            }

            return true;
        }

        private static bool UpdateProjectTclFile()
        {
            try
            {
                VerilogProject.UpdateProjectFile(String.Concat(_execPath, "\\Temp\\project.tcl"));
            }
            catch (Exception ex)
            {
                Logger.LogError(String.Concat("Could not update 'project.tcl' file.\r\n\r\nException:  ", ex.Message));
                return false;
            }

            return true;
        }

        private static bool UpdateProjectFiles(VerilogTemplateValue[] templateValues)
        {
            try
            {
                string rootPath = String.Concat(_execPath, "\\Temp\\");
                string verilogPath = String.Concat(rootPath, "verilog\\");
                string[] fileNames = new string[] {
                    String.Concat(rootPath, "constraints.ucf"),
                    String.Concat(verilogPath, "top_module.v"),
                    String.Concat(verilogPath, "processor.v"),
                    String.Concat(verilogPath, "prog_roms.v")
                };

                VerilogProject.UpdateFiles(templateValues, fileNames);
            }
            catch (Exception ex)
            {
                Logger.LogError(String.Concat("Could not update project files.\r\n\r\nException:  ", ex.Message));
                return false;
            }

            return true;
        }

        private static bool Synthesize()
        {
            try
            {
                string[] cmd = COMMAND.Replace("{ISE_DIR}", _iseDir).Replace("{PROJ_DIR}", _execPath + "\\Temp").Split('|');

                _proc = new Process();
                _proc.StartInfo.FileName = cmd[0];
                _proc.StartInfo.Arguments = cmd[1];
        
                _proc.StartInfo.UseShellExecute = false;
                _proc.StartInfo.RedirectStandardOutput = true;
                _proc.StartInfo.RedirectStandardError = true;
                _proc.StartInfo.CreateNoWindow = true;

                _proc.EnableRaisingEvents = true;
                _proc.OutputDataReceived += StandardOutputDataReceived;
                _proc.ErrorDataReceived += StandardErrorDataReceived;

                _proc.Start();
                _proc.BeginOutputReadLine();
                _proc.BeginErrorReadLine();

                StartMonitoringThread();
            }
            catch (Exception ex)
            {
                Logger.LogError(String.Concat("PlanAhead process failed.\r\n\r\nException:  ", ex.Message));
                return false;
            }

            return true;
        }

        private static void StandardOutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Logger.Input(e.Data);
        }

        private static void StandardErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            Logger.Input(e.Data);
        }

        private static void StartMonitoringThread()
        {
            _thread = new Thread(new ThreadStart(MonitoringThread));
            _thread.IsBackground = true;
            _thread.Start();
        }

        private static void MonitoringThread()
        {
            if (_proc == null) return;

            while (!_proc.HasExited)
                Thread.Sleep(500);

            TimeSpan runningTime = DateTime.Now - _startTime;
            string binFile = String.Concat(_execPath, "\\Temp\\planAhead\\FPGA_CPU\\FPGA_CPU.runs\\impl_1\\top_module.bin");

            if (File.Exists(binFile))
            {
                Logger.LogInfo(String.Concat("Bin file created successfully and is ready for upload.", System.Environment.NewLine, "Running Time: ", runningTime.ToString(@"mm\:ss", CultureInfo.InvariantCulture)));

                if (SynthesizeProcessCompleted != null)
                    SynthesizeProcessCompleted(true);
            }
            else
            {
                Logger.LogError(String.Concat("Bin file could not be created.", System.Environment.NewLine, "Running Time: ", runningTime.ToString(@"mm\:ss", CultureInfo.InvariantCulture)));

                if (SynthesizeProcessCompleted != null)
                    SynthesizeProcessCompleted(false);
            }

            _isRunning = false;
        }
        #endregion
    }
}
