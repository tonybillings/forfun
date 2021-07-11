using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace TIDE.UI.Windows
{
    public partial class ManualWindow : Window
    {
        public ManualWindow()
        {
            InitializeComponent();
            InitWebBrowser();
        }

        private void InitWebBrowser()
        {
            string curDir = Directory.GetCurrentDirectory();
            MainWebBrowser.Navigate(new Uri(String.Format("file:///{0}/Manual/index.html", curDir)));
        }
    }
}
