using ColorFont;
using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Xml;
using TIDE.Code;
using TIDE.IDE;
using TIDE.Util;

namespace TIDE.UI.Windows
{
    public partial class MainWindow : Window
    {
        #region Constants
        private const string TITLE = "TIDE : ";
        #endregion

        #region Constructors
        public MainWindow()
        {
            InitializeComponent();
            Init();
        }
        #endregion

        #region Private Methods
        private void Init()
        {
            IDE.Environment.ExecPath = Assembly.GetExecutingAssembly().Location.Substring(0, Assembly.GetExecutingAssembly().Location.LastIndexOf("\\"));

            IDE.Environment.Language = TIDE.Code.Language.xT50;
            IDE.Environment.AnimationActive = false;

            LoadHighlightingDefinitions();
            LoadSerialPorts();
            ChangeLanguage(Code.Language.xT50);
            LoadOptions();
            NewFile();

            Editor.TextChanged += Editor_TextChanged;
            Closing += MainWindow_Closing;

            Compiler.SynthesizeProcessCompleted += Compiler_SynthesizeProcessCompleted;
            Logger.DataCleared += Logger_DataCleared;
        }

        private void LoadHighlightingDefinitions()
        {
            foreach (Language lang in Enum.GetValues(typeof(Language)))
            {
                try
                {
                    XshdSyntaxDefinition xshd;
                    using (XmlTextReader reader = new XmlTextReader(IDE.Environment.ExecPath + "\\UI\\Editor\\" + lang.ToString() + ".xml"))
                        xshd = HighlightingLoader.LoadXshd(reader);
                    Editor.HighlightingDefinitions.Add(lang.ToString(), HighlightingLoader.Load(xshd, HighlightingManager.Instance));
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Could not load syntax highlighting definition for the " + lang.ToString() + " language.\r\n\r\nException:  " + ex.Message, "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void LoadSerialPorts()
        {
            string[] ports = SerialPort.GetPortNames();

            SerialPortListSeparator.Visibility = (ports.Length == 0) ? Visibility.Hidden : Visibility.Visible;

            foreach (string port in ports)
            {
                MenuItem newItem = new MenuItem() { Header = port };
                newItem.Click += SerialPortMenu_Click;
                SerialPortMenu.Items.Insert(0, newItem);
            }
        }

        private void ChangeLanguage(Language lang)
        {
            ChangeLanguage(lang.ToString());
        }

        private void ChangeLanguage(string language)
        {
            EnableSyntaxHighlighting(language);

            foreach (Language lang in Enum.GetValues(typeof(Language)))
            {
                if (lang.ToString() == language)
                    IDE.Environment.Language = lang;
            }
        }

        private void EnableSyntaxHighlighting(Language lang)
        {
            EnableSyntaxHighlighting(lang.ToString());
        }

        private void EnableSyntaxHighlighting(string lang)
        {
            IHighlightingDefinition result;
            Editor.HighlightingDefinitions.TryGetValue(lang, out result);

            if (result != null)
            {
                Editor.SyntaxHighlighting = result;

                foreach (MenuItem item in LanguageMenu.Items)
                {
                    if (item.Header.ToString() == lang)
                        item.IsChecked = true;
                    else
                        item.IsChecked = false;
                }
            }
        }

        private string GetFileExtension(Language lang, bool withDot = true)
        {
            string result = (withDot) ? "." : "";

            switch (lang)
            {
                case TIDE.Code.Language.xT50:
                    return result + "xt50";
                case TIDE.Code.Language.Tranquil:
                    return result + "trql";
                default:
                    return String.Empty;
            }
        }

        private void NewFile()
        {
            if (IDE.Environment.Unsaved)
            {
                switch (MessageBox.Show("Save file first?", "", MessageBoxButton.YesNoCancel))
                {
                    case MessageBoxResult.Yes:
                        SaveFile();
                        break;
                    case MessageBoxResult.No:
                        // do nothing
                        break;
                    case MessageBoxResult.Cancel:
                        return;
                }
            }
            
            Editor.Clear();
            Title = TITLE + "NewFile";
            IDE.Environment.Filename = null;
            IDE.Environment.Unsaved = false;
        }

        private void OpenFile()
        {
            if (IDE.Environment.Unsaved)
            {
                switch (MessageBox.Show("Save file first?", "", MessageBoxButton.YesNoCancel))
                {
                    case MessageBoxResult.Yes:
                        SaveFile();
                        break;
                    case MessageBoxResult.No:
                        // do nothing
                        break;
                    case MessageBoxResult.Cancel:
                        return;
                }
            }

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = false;
            ofd.CheckPathExists = true;
            ofd.InitialDirectory = (IDE.Environment.Directory == null) ? System.Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop) : IDE.Environment.Directory;
            ofd.Title = "Open file...";

            if (ofd.ShowDialog() == true)
            {
                IDE.Environment.Directory = ofd.FileName.Substring(0, ofd.FileName.LastIndexOf("\\"));
                Editor.Text = File.ReadAllText(ofd.FileName);
                string ext = ofd.SafeFileName.Substring(ofd.SafeFileName.LastIndexOf('.'));

                foreach (Language lang in Enum.GetValues(typeof(Language)))
                {
                    if (ext == GetFileExtension(lang))
                    {
                        ChangeLanguage(lang);
                        break;
                    }
                }

                Title = TITLE + ofd.SafeFileName.Substring(0, ofd.SafeFileName.LastIndexOf('.'));
                IDE.Environment.Filename = ofd.FileName;
                IDE.Environment.Unsaved = false;
            }
        }

        private void SaveFile()
        {
            if (IDE.Environment.Filename == null)
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.CheckPathExists = sfd.OverwritePrompt = true;
                sfd.DefaultExt = GetFileExtension(IDE.Environment.Language);
                sfd.Filter = IDE.Environment.Language.ToString() + " | *." + sfd.DefaultExt;
                sfd.InitialDirectory = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop);
                sfd.Title = "Save file...";

                if (sfd.ShowDialog() == true)
                {
                    File.WriteAllText(sfd.FileName, Editor.Text);
                    Title = TITLE + sfd.SafeFileName.Substring(0, sfd.SafeFileName.LastIndexOf('.'));
                    IDE.Environment.Filename = sfd.FileName;
                    IDE.Environment.Unsaved = false;
                }
            }
            else
            {
                File.WriteAllText(IDE.Environment.Filename, Editor.Text);
                Title = Title.TrimEnd("*".ToCharArray());
                IDE.Environment.Unsaved = false;
            }
        }

        private void SaveFileAs()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.CheckPathExists = sfd.OverwritePrompt = true;
            sfd.DefaultExt = GetFileExtension(IDE.Environment.Language);
            sfd.Filter = IDE.Environment.Language.ToString() + " | *." + sfd.DefaultExt;
            sfd.InitialDirectory = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop);
            sfd.Title = "Save file as...";

            if (sfd.ShowDialog() == true)
            {
                File.WriteAllText(sfd.FileName, Editor.Text);
                Title = TITLE + sfd.SafeFileName.Substring(0, sfd.SafeFileName.LastIndexOf('.'));
                IDE.Environment.Filename = sfd.FileName;
                IDE.Environment.Unsaved = false;
            }
        }

        private void SaveOptions()
        {
            Editor.SetOptions();
            Uploader.SetOptions();
            Compiler.SetOptions();
            IDE.Environment.SetOptions();
           
            string fileName = String.Concat(IDE.Environment.ExecPath, "\\Options.cfg");
            Options.Save(fileName);
        }

        private void LoadOptions()
        {
            string fileName = String.Concat(IDE.Environment.ExecPath, "\\Options.cfg");
            if (!Options.Load(fileName)) return;

            Editor.GetOptions();
            Uploader.GetOptions();
            Compiler.GetOptions();
            IDE.Environment.GetOptions();

            OptionsChanged();
        }

        private void OptionsChanged()
        {
            for (int i = 0; i < SerialPortMenu.Items.Count; i++)
            {
                MenuItem item = SerialPortMenu.Items[i] as MenuItem;
                if (item == null) continue;

                if (item.Header.ToString() == Uploader.SerialPortName)
                {
                    item.IsChecked = true;
                    Uploader.SerialPortName = Uploader.SerialPortName;
                    break;
                }
            }

            ChangeLanguage(IDE.Environment.Language);

            AutoSizeOutputWindowMenu.IsChecked = IDE.Environment.AutoSizeOutputWindowMenu;
        }

        private void IndentTextRight()
        {
            try
            {
                TextDocument document = Editor.Document;
                DocumentLine start = document.GetLineByOffset(Editor.SelectionStart);
                DocumentLine end = document.GetLineByOffset(Editor.SelectionStart + Editor.SelectionLength).NextLine;

                using (document.RunUpdate())
                {
                    for (DocumentLine line = start; line != end; line = line.NextLine)
                    {
                        document.Insert(line.Offset, "\t");
                    }
                }
            }
            catch { }
        }

        private void IndentTextLeft()
        {
            try
            {
                TextDocument document = Editor.Document;
                DocumentLine start = document.GetLineByOffset(Editor.SelectionStart);
                DocumentLine end = document.GetLineByOffset(Editor.SelectionStart + Editor.SelectionLength).NextLine;

                using (document.RunUpdate())
                {
                    for (DocumentLine line = start; line != end; line = line.NextLine)
                    {
                        if (document.GetText(line.Offset, 1) == "\t")
                            document.Remove(line.Offset, 1);
                    }
                }
            }
            catch { }
        }

        private void CommentSelection()
        {
            try
            {
                TextDocument document = Editor.Document;
                DocumentLine start = document.GetLineByOffset(Editor.SelectionStart);
                DocumentLine end = document.GetLineByOffset(Editor.SelectionStart + Editor.SelectionLength).NextLine;

                using (document.RunUpdate())
                {
                    for (DocumentLine line = start; line != end; line = line.NextLine)
                    {
                        document.Insert(line.Offset, "#");
                    }
                }
            }
            catch { }
        }

        private void UncommentSelection()
        {
            try
            {
                TextDocument document = Editor.Document;
                DocumentLine start = document.GetLineByOffset(Editor.SelectionStart);
                DocumentLine end = document.GetLineByOffset(Editor.SelectionStart + Editor.SelectionLength).NextLine;

                using (document.RunUpdate())
                {
                    for (DocumentLine line = start; line != end; line = line.NextLine)
                    {
                        if (document.GetText(line.Offset, 1) == "#")
                            document.Remove(line.Offset, 1);
                    }
                }
            }
            catch { }
        }

        private void OpenManual()
        {
            ManualWindow mw = new ManualWindow();
            mw.ShowDialog();
        }

        private void Validate()
        {
            if (Uploader.IsRunning || Compiler.IsRunning || IDE.Environment.AnimationActive || Editor.Text.Trim().Length == 0) return;
            if (IDE.Environment.AutoSizeOutputWindowMenu) MaximizeOutputWindow();
            Compiler.Validate(IDE.Environment.Language, Editor.Text);
        }

        private void Compile()
        {
            if (Uploader.IsRunning || Compiler.IsRunning || IDE.Environment.AnimationActive || Editor.Text.Trim().Length == 0) return;
            if (IDE.Environment.AutoSizeOutputWindowMenu) MaximizeOutputWindow();
            Compiler.Exec(IDE.Environment.Language, Editor.Text);
        }

        private void Upload(bool storeInFlash)
        {
            if (Compiler.IsRunning || Uploader.IsRunning || IDE.Environment.AnimationActive) return;
            if (IDE.Environment.AutoSizeOutputWindowMenu) MaximizeOutputWindow();
            Uploader.Upload(storeInFlash);
        }

        private void MaximizeOutputWindow()
        {
            if (IDE.Environment.AnimationActive) return;
            double maxHeight = MainGrid.ActualHeight - (MainMenu.ActualHeight + MainToolBar.ActualHeight);
            var da = new GridLengthAnimation();
            da.Completed += Animation_Completed;
            da.From = new GridLength(OutputWindowRow.ActualHeight);
            da.To = new GridLength(maxHeight);
            da.AccelerationRatio = .5f;
            da.DecelerationRatio = .5f;
            da.FillBehavior = FillBehavior.Stop;
            da.Duration = new Duration(TimeSpan.FromMilliseconds(600));
            OutputWindowRow.Height = new GridLength(maxHeight);
            OutputWindowRow.BeginAnimation(RowDefinition.HeightProperty, da);
            IDE.Environment.AnimationActive = true;
        }

        private void MinimizeOutputWindow()
        {
            if (IDE.Environment.AnimationActive) return;
            var da = new GridLengthAnimation();
            da.Completed += Animation_Completed;
            da.From = new GridLength(OutputWindowRow.ActualHeight);
            da.To = new GridLength(OutputWindowRow.MinHeight);
            da.AccelerationRatio = .5f;
            da.DecelerationRatio = .5f;
            da.FillBehavior = FillBehavior.Stop;
            da.Duration = new Duration(TimeSpan.FromMilliseconds(600));
            OutputWindowRow.Height = new GridLength(OutputWindowRow.MinHeight);
            OutputWindowRow.BeginAnimation(RowDefinition.HeightProperty, da);
            IDE.Environment.AnimationActive = true;
        }
        #endregion

        #region Event Handlers
        private void NewButton_Click(object sender, RoutedEventArgs e)
        {
            NewFile();
        }

        private void NewMenuItem_Click(object sender, RoutedEventArgs e)
        {
            NewFile();
        }

        private void OpenMenuItem_Click(object sender, RoutedEventArgs e)
        {
            OpenFile();
        }

        private void OpenButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFile();
        }

        private void SaveMenuItem_Click(object sender, RoutedEventArgs e)
        {
            SaveFile();
        }

        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFile();
        }

        private void SaveAsMenuItem_Click(object sender, RoutedEventArgs e)
        {
            SaveFileAs();
        }

        private void ExitMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void UndoMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Undo();
        }

        private void CutMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Cut();
        }

        private void CopyMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Copy();
        }

        private void PasteMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Paste();
        }

        private void DeleteMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.SelectedText = String.Empty;
        }

        private void SelectAllMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.SelectAll();
        }

        private void IndentRightMenuItem_Click(object sender, RoutedEventArgs e)
        {
            IndentTextRight();
        }

        private void IndentLeftMenuItem_Click(object sender, RoutedEventArgs e)
        {
            IndentTextLeft();
        }

        private void CommentMenuItem_Click(object sender, RoutedEventArgs e)
        {
            CommentSelection();
        }

        private void UncommentMenuItem_Click(object sender, RoutedEventArgs e)
        {
            UncommentSelection();
        }

        private void ValidateButton_Click(object sender, RoutedEventArgs e)
        {
            Validate();
        }

        private void ValidateItem_Click(object sender, RoutedEventArgs e)
        {
            Validate();
        }

        private void CompileButton_Click(object sender, RoutedEventArgs e)
        {
            Compile();
        }

        private void CompileMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Compile();
        }

        private void UploadRamMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Upload(false);
        }

        private void UploadFlashMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Upload(true);
        }

        private void UploadButton_Click(object sender, RoutedEventArgs e)
        {
            Upload(false);
        }

        private void FontMenuItem_Click(object sender, RoutedEventArgs e)
        {
            ColorFontDialog cfd = new ColorFontDialog();
            cfd.Owner = this;
            cfd.Font = FontInfo.GetControlFont(Editor);

            if (cfd.ShowDialog() == true)
            {
                if (cfd.Font != null)
                {
                    Editor.FontFamily = cfd.Font.Family;
                    Editor.FontSize = cfd.Font.Size;
                }
            }
        }

        private void LanguageMenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem item = sender as MenuItem;
            if (item == null) return;
            ChangeLanguage(item.Header.ToString());
        }

        private void SerialPortMenu_Click(object sender, RoutedEventArgs e)
        {
            MenuItem clickedItem = sender as MenuItem;
            if (clickedItem == null) return;

            for (int i = 0; i < SerialPortMenu.Items.Count; i++)
            {
                MenuItem item = SerialPortMenu.Items[i] as MenuItem;
                if (item == null) continue;
                item.IsChecked = false;
            }

            clickedItem.IsChecked = true;
            Uploader.SerialPortName = clickedItem.Header.ToString();
        }

        private void SerialRefreshMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Uploader.SerialPortName = String.Empty;

            for (int i = SerialPortMenu.Items.Count - 3; i >= 0; i--)
                SerialPortMenu.Items.RemoveAt(i);

            LoadSerialPorts();
        }

        private void IseDirectoryMenu_Click(object sender, RoutedEventArgs e)
        {
            IseDirectoryWindow idw = new IseDirectoryWindow();
            idw.IseDirectory = Compiler.IseDirectory;

            if (idw.ShowDialog() == true)
                Compiler.IseDirectory = idw.IseDirectory;
        }

        private void AutoSizeOutputWindowMenu_Click(object sender, RoutedEventArgs e)
        {
            AutoSizeOutputWindowMenu.IsChecked = IDE.Environment.AutoSizeOutputWindowMenu = !AutoSizeOutputWindowMenu.IsChecked;
        }

        private void ManualMenuItem_Click(object sender, RoutedEventArgs e)
        {
            OpenManual();
        }

        private void AboutMenuItem_Click(object sender, RoutedEventArgs e)
        {
            AboutWindow aw = new AboutWindow();
            aw.ShowDialog();
        }

        private void UndoContextMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Undo();
        }

        private void CutContextMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Cut();
        }

        private void CopyContextMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Copy();
        }

        private void PasteContextMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.Paste();
        }

        private void DeleteContextMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.SelectedText = String.Empty;
        }

        private void SelectAllContextMenuItem_Click(object sender, RoutedEventArgs e)
        {
            Editor.SelectAll();
        }

        private void Editor_TextChanged(object sender, EventArgs e)
        {
            if (!IDE.Environment.Unsaved)
            {
                IDE.Environment.Unsaved = true;
                Title = Title.Replace("*", String.Empty) + "*";
            }
        }

        private void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (IDE.Environment.Unsaved)
            {
                switch (MessageBox.Show("Save before exiting?", "", MessageBoxButton.YesNoCancel))
                {
                    case MessageBoxResult.Yes:
                        SaveFile();
                        break;
                    case MessageBoxResult.No:
                        e.Cancel = false;
                        break;
                    case MessageBoxResult.Cancel:
                        e.Cancel = true;
                        return;
                }
            }

            SaveOptions();
        }

        private void Editor_KeyDown(object sender, KeyEventArgs e)
        {
            if (Keyboard.IsKeyDown(Key.LeftCtrl))
            {
                switch (e.Key)
                {
                    case Key.LeftAlt:
                        CommentSelection();
                        break;
                    case Key.LeftShift:
                        UncommentSelection();
                        break;
                    case Key.N:
                        NewFile();
                        e.Handled = true;
                        return;
                    case Key.O:
                        OpenFile();
                        e.Handled = true;
                        return;
                    case Key.S:
                        SaveFile();
                        e.Handled = true;
                        return;
                    case Key.Q:
                        Close();
                        return;
                }
            }
            else
            {
                switch (e.Key)
                {
                    case Key.F5:
                    case Key.F6:
                    case Key.F7:
                    case Key.F8:
                        return;
                }
            }

            if (IDE.Environment.AutoSizeOutputWindowMenu && OutputWindowRow.ActualHeight != OutputWindowRow.MinHeight)
            {
                MinimizeOutputWindow();
            }
        }

        private void OutputTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            OutputTextbox.ScrollToEnd();
        }

        private void OutputTextbox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (Uploader.IsRunning || Compiler.IsRunning) return;
            MinimizeOutputWindow();
        }

        private void Animation_Completed(object sender, EventArgs e)
        {
            IDE.Environment.AnimationActive = false;
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.IsRepeat) return;

            switch (e.Key)
            {
                case Key.F1:
                    OpenManual();
                    break;
                case Key.F5:
                    Validate();
                    break;
                case Key.F6:
                    Compile();
                    break;
                case Key.F7:
                    Upload(false);
                    break;
                case Key.F8:
                    Upload(true);
                    break;
            }
        }

        private void Compiler_SynthesizeProcessCompleted(bool wasSuccessful)
        {
            this.Dispatcher.Invoke((Action)(() =>
            {
                ViewErrorsWarningsButton.Opacity = 0;
                ViewErrorsWarningsButton.Visibility = Visibility.Visible;

                var animation = new DoubleAnimation
                {
                    To = 0.5f,
                    BeginTime = TimeSpan.FromSeconds(0),
                    Duration = TimeSpan.FromSeconds(0.5f),
                    FillBehavior = FillBehavior.Stop
                };

                animation.Completed += (s, a) => ViewErrorsWarningsButton.Opacity = 0.5f;

                ViewErrorsWarningsButton.BeginAnimation(UIElement.OpacityProperty, animation);
            }));
        }

        private void Logger_DataCleared()
        {
            this.Dispatcher.Invoke((Action)(() =>
            {
                ViewErrorsWarningsButton.Visibility = Visibility.Hidden;
            }));
        }

        private void ViewErrorsWarningsButton_Click(object sender, RoutedEventArgs e)
        {
            ErrorsWarningsWindow window = new ErrorsWarningsWindow();
            window.ParseLogText(OutputTextbox.Text);
            window.ShowDialog();
        }
        #endregion
    }
}
