using System;
using System.IO;

namespace TIDE.Code
{
    public static class VerilogProject
    {
        #region Public Methods
        public static void UpdateProjectFile(string fileName)
        {
            FileInfo fileInfo = new FileInfo(fileName);
            string fileContent = File.ReadAllText(fileName);
            fileContent = fileContent.Replace("{FPGA_DIR}", fileInfo.DirectoryName.Replace("\\", "/").Replace(" ", "\\ "));
            File.WriteAllText(fileName, fileContent);
        }

        public static void UpdateFiles(VerilogTemplateValue[] values, params string[] fileNames)
        {
            foreach (string fileName in fileNames)
            {
                FileInfo fileInfo = new FileInfo(fileName);
                string fileContent = File.ReadAllText(fileName);

                foreach (VerilogTemplateValue value in values)
                    fileContent = fileContent.Replace(String.Concat("///", value.PlaceholderText, "///"), value.PlaceholderValue);

                File.WriteAllText(fileName, fileContent);
            }
        }
        #endregion

    }
}
