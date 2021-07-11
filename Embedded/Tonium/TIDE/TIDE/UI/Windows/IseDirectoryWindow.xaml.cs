using System.Windows;
using System.Windows.Forms;

namespace TIDE.UI.Windows
{
    public partial class IseDirectoryWindow : Window
    {
        #region Properties
        public string IseDirectory
        {
            get
            {
                return DirectoryTextBox.Text;
            }
            set
            {
                DirectoryTextBox.Text = value;
            }
        }
        #endregion

        #region Constructors
        public IseDirectoryWindow()
        {
            InitializeComponent();
        }
        #endregion

        #region Event Handlers
        private void BrowseButton_Click(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Select folder...";
            fbd.ShowNewFolderButton = false;
            fbd.RootFolder = System.Environment.SpecialFolder.MyComputer;

            if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                DirectoryTextBox.Text = fbd.SelectedPath;
        }

        private void OkayButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }
        #endregion


    }
}
