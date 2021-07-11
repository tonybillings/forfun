using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RobotMapper
{
    public partial class PortSelector : Form
    {
        #region Public Variables
        public string ComPort;
        #endregion

        #region Constructors
        public PortSelector()
        {
            InitializeComponent();
        }
        #endregion

        #region Form Events
        private void PortSelector_Load(object sender, EventArgs e)
        {
            GetAvailablePorts();
        }
        #endregion

        #region Control Events
        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (cmbPorts.SelectedIndex == -1)
            {
                lblWarning.Visible = true;
                return;
            }

            lblWarning.Visible = false;
            ComPort = cmbPorts.SelectedItem.ToString();
            this.DialogResult = DialogResult.OK;
        }

        private void cmbPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            lblWarning.Visible = false;
        }
        #endregion

        #region Methods
        private void GetAvailablePorts()
        {
            string[] ports = SerialPort.GetPortNames();

            foreach (string port in ports)
                cmbPorts.Items.Add(port);
        }
        #endregion

    }
}
