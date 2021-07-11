using MathNet.Numerics.IntegralTransforms;
using MathNet.Numerics.IntegralTransforms.Algorithms;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace FourierTransform
{
    public partial class Main : Form
    {
        #region Constructors
        public Main()
        {
            InitializeComponent();        
        }
        #endregion

        #region Control Events
        private void addWaveButton_Click(object sender, EventArgs e)
        {
            AddWave();
        }

        private void compositeVisibleCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            graphControl1.CompositeVisible = compositeVisibleCheckbox.Checked;
        }

        private void waveControl_DeleteClicked(WaveControl sender)
        {
            for (int i = flowLayoutPanel1.Controls.Count - 1; i >= 0; i--)
            {
                if (flowLayoutPanel1.Controls[i] == sender)
                {
                    flowLayoutPanel1.Controls.Remove(sender);
                    return;
                }
            }
        }
        #endregion

        #region Form Events
        private void Main_Load(object sender, EventArgs e)
        {
            AddWave();
        }
        #endregion

        #region Methods
        private void AddWave()
        {
            WaveControl wave = new WaveControl();
            wave.DeleteClicked += waveControl_DeleteClicked;
            flowLayoutPanel1.Controls.Add(wave);
            graphControl1.AddWave(wave);
        }
        #endregion

        private void freqDomainRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (freqDomainRadioButton.Checked)
                chart1.Visible = true;
        }

        private void timeDomainRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (timeDomainRadioButton.Checked)
                chart1.Visible = false;
        }

        private void ShowTimeDomain()
        {
            chart1.Visible = false;
        }

        private void UpdateFrequencyDomainChart()
        {
            VertexPositionColor[] points = graphControl1.CompositePoints;
            DiscreteFourierTransform transform = new DiscreteFourierTransform();
            Complex[] samples = new Complex[points.Length];

            for (int i = 0; i < points.Length; i++)
                samples[i] = new Complex((double)points[i].Position.Y, 0);

            transform.Radix2Forward(samples, FourierOptions.AsymmetricScaling);

            chart1.Series["Series1"].Points.Clear();

            for (int i = 0; i < samples.Length / 2; i++)
                chart1.Series["Series1"].Points.AddY(samples[i].Magnitude);
        } 

        private void graphControl1_DataChanged(GraphControl sender)
        {
            UpdateFrequencyDomainChart();
        }
    }
}
