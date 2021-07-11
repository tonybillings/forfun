using RobotMapper.Messaging;
using System;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace RobotMapper
{
    public partial class Main : Form
    {
        #region Constants
        public const int BAUD_RATE = 9600;
        #endregion

        #region Type Definitions
        private enum MessageType : byte { PosRotUpdate = 100, NewMarker = 101 }
        #endregion

        #region Private Variables
        private SerialPort _serial;
        #endregion

        #region Constructors
        public Main()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
        }
        #endregion

        #region Form Events
        private void Main_Load(object sender, EventArgs e)
        {
            InitializeControls();
            InitializeSerial();
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_serial != null && _serial.IsOpen)
                _serial.Close();
        }
        #endregion

        #region Control Events
        private void chkDrawMarkers_CheckedChanged(object sender, EventArgs e)
        {
            xnaControl.MarkersEnabled = chkDrawMarkers.Checked;
        }

        private void chkDrawLines_CheckedChanged(object sender, EventArgs e)
        {
            xnaControl.LinesEnabled = chkDrawLines.Checked;
        }

        private void chkDrawGrid_CheckedChanged(object sender, EventArgs e)
        {
            xnaControl.GridEnabled = chkDrawGrid.Checked;
        }

        private void chkDrawRobot_CheckedChanged(object sender, EventArgs e)
        {
            xnaControl.RobotEnabled = chkDrawRobot.Checked;
        }

        private void chkDrawPath_CheckedChanged(object sender, EventArgs e)
        {
            xnaControl.PathEnabled = chkDrawPath.Checked;
        }

        private void sliderMarkerConnectionRadius_Scroll(object sender, EventArgs e)
        {
            xnaControl.MarkerConnectionRadius = sliderMarkerConnectionRadius.Value;
        }
        #endregion

        #region Messaging Events
        private void MessageParser_MessageReceived(byte messageType, byte[] payload)
        {
            MessageType msgType = (MessageType)messageType;

            switch (msgType)
            {
                case MessageType.PosRotUpdate:
                    HandlePosRotUpdate(payload);
                    break;
                case MessageType.NewMarker:
                    HandleNewMarkerUpdate(payload);
                    break;
            }
        }

        private void HandlePosRotUpdate(byte[] payload)
        {
            byte[] xArray = { payload[0], payload[1], payload[2], payload[3], 46, payload[4], payload[5] };
            byte[] yArray = { payload[6], payload[7], payload[8], payload[9], 46, payload[10], payload[11] };
            byte[] rotArray = { payload[12], payload[13], payload[14], payload[15], 46, payload[16], payload[17] };
            double x = Convert.ToDouble(ASCIIEncoding.ASCII.GetString(xArray));
            double y = Convert.ToDouble(ASCIIEncoding.ASCII.GetString(yArray));
            double rot = Convert.ToDouble(ASCIIEncoding.ASCII.GetString(rotArray));
            xnaControl.UpdateRobotPosition(x, y);
            xnaControl.UpdateRobotRotation(rot);
        }

        private void HandleNewMarkerUpdate(byte[] payload)
        {

        }
        #endregion

        #region Methods
        private void InitializeControls()
        {
            System.Drawing.Color backColor = System.Drawing.Color.FromArgb(63, 72, 204);
            chkDrawGrid.BackColor = backColor;
            chkDrawMarkers.BackColor = backColor;
            chkDrawLines.BackColor = backColor;
            chkDrawRobot.BackColor = backColor;
            chkDrawPath.BackColor = backColor;
            lblMarkerConnectionRadius.BackColor = backColor;
            sliderMarkerConnectionRadius.BackColor = backColor;
        }

        private void InitializeSerial()
        {
            PortSelector form = new PortSelector();

            try
            {
                if (form.ShowDialog() == DialogResult.OK && !String.IsNullOrEmpty(form.ComPort))
                {
                    _serial = new SerialPort();
                    _serial.PortName = form.ComPort;
                    _serial.BaudRate = BAUD_RATE;
                    _serial.Parity = Parity.None;
                    _serial.DataBits = 8;
                    _serial.StopBits = StopBits.One;
                    _serial.Handshake = Handshake.None;
                    _serial.Open();
                    Thread.Sleep(2000);
                    if (!_serial.IsOpen) { return; }
                    Thread t = new Thread(new ThreadStart(ReadSerial));
                    t.Start();
                    MessageParser.MessageReceived += MessageParser_MessageReceived;
                }
                else
                {

                    MessageBox.Show("Application will continue in DEBUG mode!");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + Environment.NewLine + Environment.NewLine + "Application will continue in DEBUG mode!", "Serial COM Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            form.Dispose();
        }

        private void ReadSerial()
        {
            _serial.DiscardInBuffer();

            while (_serial.IsOpen)
            {
                if (_serial.BytesToRead == 0) { continue; }
                MessageParser.ProcessByte((byte)_serial.ReadByte());
            }
        }
        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            Commands form = new Commands();
            form.Show();
        }

 
    }
}
