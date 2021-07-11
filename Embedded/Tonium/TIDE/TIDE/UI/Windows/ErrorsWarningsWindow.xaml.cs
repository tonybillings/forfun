using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;

namespace TIDE.UI.Windows
{
    public partial class ErrorsWarningsWindow : Window
    {
        #region Nested Types
        private class Row
        {
            public int ID { get; set; }
            public string Description { get; set; }
            public bool IsCritical { get; set; }
        }
        #endregion

        #region Constructors
        public ErrorsWarningsWindow()
        {
            InitializeComponent();
        }
        #endregion

        #region Public Methods
        public void ParseLogText(string text)
        {
            ClearDataGrids();

            string[] lines = text.Split(Environment.NewLine.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

            ErrorsDataGrid.ItemsSource = GetDataGridRows(lines, "ERROR:");
            WarningsDataGrid.ItemsSource = GetDataGridRows(lines, "WARNING:");
            InfoDataGrid.ItemsSource = GetDataGridRows(lines, "INFO:");

            ErrorsTabText.Text += String.Concat(" (", ErrorsDataGrid.Items.Count - 1, ")");
            WarningsTabText.Text += String.Concat(" (", WarningsDataGrid.Items.Count - 1, ")");
            InfoTabText.Text += String.Concat(" (", InfoDataGrid.Items.Count - 1, ")");
        }
        #endregion

        #region Private Methods
        private void ClearDataGrids()
        {
            ErrorsDataGrid.Items.Clear();
            WarningsDataGrid.Items.Clear();
            InfoDataGrid.Items.Clear();
        }

        private List<Row> GetDataGridRows(string[] logTextLines, string prefix)
        {
            string critPrefix = String.Concat("CRITICAL ", prefix);
            List<Row> rows = new List<Row>();
            int i = 1;
            bool isMultiLine = false;

            foreach (string line in logTextLines)
            {
                if (line.StartsWith(prefix))
                {
                    rows.Add(new Row { ID = i++, Description = line.Substring(line.IndexOf(":") + 1).Trim() });
                    isMultiLine = true;
                }
                else if (line.StartsWith(critPrefix))
                {
                    rows.Add(new Row { ID = i++, Description = line.Substring(line.IndexOf(":") + 1).Trim(), IsCritical = true });
                    isMultiLine = true;
                }
                else if (isMultiLine && line.StartsWith("  "))
                {
                    rows[rows.Count - 1].Description += String.Concat(" ", line.Trim());
                }
                else
                {
                    isMultiLine = false;
                }
            }

            return rows;
        }
        #endregion

        #region Event Handlers
        private void MainTabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            TabControl tc = e.Source as TabControl;
            if (tc == null) return;
            TabItem ti = tc.SelectedItem as TabItem;
            if (ti == null) return;

            switch (ti.Name)
            {
                case "ErrorsTab":
                    Title = "Errors";
                    break;
                case "WarningsTab":
                    Title = "Warnings";
                    break;
                case "InfoTab":
                    Title = "Info";
                    break;
            }
        }
        #endregion
    }
}
