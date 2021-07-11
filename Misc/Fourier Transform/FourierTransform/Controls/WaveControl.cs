using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace FourierTransform
{
    public partial class WaveControl : UserControl
    {
        #region Events
        public delegate void DeleteClickedHandler(WaveControl wave);
        public event DeleteClickedHandler DeleteClicked;
        public delegate void DataChangedHandler(WaveControl wave);
        public event DataChangedHandler DataChanged;
        public delegate void WaveEnabledChangedHandler(WaveControl wave);
        public event WaveEnabledChangedHandler WaveEnabledChanged;
        #endregion

        #region Constants
        public static int PRIMITIVE_COUNT = 8191;
        #endregion

        #region Declarations
        VertexPositionColor[] _points;
        VertexPositionColor[] _displayPoints;
        private float _frequency;
        private float _amplitude;
        private float _phase;
        float _height;
        float _width;
        float _halfHeight;
        float _halfWidth;
        float[] _amplitudes;
        bool _waveEnabled;
        bool _waveVisible;
        bool _useSliders;
        #endregion

        #region Constructors
        public WaveControl()
        {
            InitializeComponent();
        }
        #endregion

        #region Properties
        public float Frequency
        {
            get { return _frequency; }
            set { _frequency = value; }
        }

        public float Amplitude
        {
            get { return _amplitude; }
            set { _amplitude = value; }
        }

        public float Phase
        {
            get { return _phase; }
            set { _phase = value; }
        }

        public VertexPositionColor[] Points
        {
            get { return _points; }
        }

        public VertexPositionColor[] DisplayPoints
        {
            get { return _displayPoints; }
        }

        public bool WaveEnabled 
        {
            get 
            {
                return _waveEnabled;
            }
            set
            {
                _waveEnabled = value;
                if (WaveEnabledChanged != null)
                    WaveEnabledChanged(this);
            }
        }

        public bool WaveVisible
        {
            get { return _waveVisible; }
            set { _waveVisible = value; }
        }

        public bool UseSliders
        {
            get { return _useSliders; }
            set { _useSliders = value; }
        }
        #endregion

        #region Wave
        public void Initialize(float width, float height)
        {
            _frequency = 1f;
            _amplitude = 1f;
            _phase = 0f;
            _amplitudes = new float[]
            {
                0.0909090909f,
                0.1f,
                0.1111111111f,
                0.125f,
                0.1428571428f,
                0.1666666666f,
                0.2f,
                0.25f,
                0.3333333333f,
                0.5f,
                1f,
                2f,
                3f,
                4f,
                5f,
                6f,
                7f,
                8f,
                9f,
                10f,
                11f
            };

            _height = height;
            _width = width;
            _halfHeight = height / 2;
            _halfWidth = width / 2;

            _waveEnabled = true;
            _waveVisible = true;
            _useSliders = true;
            UpdateWave();
        }

        public void UpdateWave()
        {
            int count = PRIMITIVE_COUNT + 1;
            _points = new VertexPositionColor[count];
            _displayPoints = new VertexPositionColor[count];
            float x = 0f;
            
            for (int i = 0; i < count; i++)
            {
                x += 0.1f;
                float y = (float)(Math.Sin((_frequency * x) / 10 + _phase) * _amplitude * -10);

                _points[i] = new VertexPositionColor() { Position = new Vector3(x, y, 0), Color = new Color(this.BackColor.R, this.BackColor.G, this.BackColor.B) };
                _displayPoints[i] = new VertexPositionColor() { Position = new Vector3(x - 51.5f, y + _halfHeight, 0), Color = new Color(this.BackColor.R, this.BackColor.G, this.BackColor.B) };
            }

            if (DataChanged != null)
                DataChanged(this);
        }
        #endregion

        #region Trackbars
        private void freqTrackBar_ValueChanged(object sender, EventArgs e)
        {
            _frequency = freqTrackBar.Value;
            freqLabel.Text = String.Format("({0})", _frequency.ToString());
            UpdateWave();
        }

        private void ampTrackBar_ValueChanged(object sender, EventArgs e)
        {
            _amplitude = _amplitudes[ampTrackBar.Value];
            string amp = "";

            if (_amplitude < 1)
                amp = "1/" + (11 - ampTrackBar.Value).ToString();
            else
                amp = _amplitude.ToString();

            ampLabel.Text = String.Format("({0})", amp);
            UpdateWave();
        }

        private void phaseTrackBar_ValueChanged(object sender, EventArgs e)
        {
            _phase = phaseTrackBar.Value;
            phaseLabel.Text = String.Format("({0})", _phase.ToString());
            UpdateWave();
        }
        #endregion

        #region Textboxes
        private void freqTextbox_TextChanged(object sender, EventArgs e)
        {
            float val;
            if (float.TryParse(freqTextbox.Text, out val))
            {
                freqTextbox.ForeColor = System.Drawing.Color.Black;
                _frequency = val;
                UpdateWave();
            }
            else
            {
                freqTextbox.ForeColor = System.Drawing.Color.Red;
            }
        }

        private void ampTextbox_TextChanged(object sender, EventArgs e)
        {
            float val;
            if (float.TryParse(ampTextbox.Text, out val))
            {
                ampTextbox.ForeColor = System.Drawing.Color.Black;
                _amplitude = val;
                UpdateWave();
            }
            else
            {
                ampTextbox.ForeColor = System.Drawing.Color.Red;
            }
        }

        private void phaseTextbox_TextChanged(object sender, EventArgs e)
        {
            float val;
            if (float.TryParse(phaseTextbox.Text, out val))
            {
                phaseTextbox.ForeColor = System.Drawing.Color.Black;
                _phase = val;
                UpdateWave();
            }
            else
            {
                phaseTextbox.ForeColor = System.Drawing.Color.Red;
            }
        }
        #endregion

        #region Controls
        private void enabledCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            WaveEnabled = !WaveEnabled;
        }

        private void visibleCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            WaveVisible = !WaveVisible;
        }

        private void slidersCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            UseSliders = !UseSliders;

            if (UseSliders)
            {
                freqTrackBar.Value = 1;
                ampTrackBar.Value = 10;
                phaseTrackBar.Value = 0;
                _frequency = 1;
                _amplitude = 1;
                _phase = 0;
                trackbarPanel.Visible = true;
                textboxPanel.Visible = false;
                UpdateWave();
            }
            else
            {
                freqTextbox.Text = "1";
                ampTextbox.Text = "1";
                phaseTextbox.Text = "0";
                _frequency = 1;
                _amplitude = 1;
                _phase = 0;
                textboxPanel.Visible = true;
                trackbarPanel.Visible = false;
                UpdateWave();
            }
        }

        private void colorLabel_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                this.BackColor = colorDialog1.Color;
                UpdateWave();
            }
        }

        private void deleteLabel_Click(object sender, EventArgs e)
        {
            if (DeleteClicked != null)
                DeleteClicked(this);
        }
        #endregion


    }
}
