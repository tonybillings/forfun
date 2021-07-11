namespace FourierTransform
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.addWaveButton = new System.Windows.Forms.Button();
            this.compositeVisibleCheckbox = new System.Windows.Forms.CheckBox();
            this.freqDomainRadioButton = new System.Windows.Forms.RadioButton();
            this.timeDomainRadioButton = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.graphControl1 = new FourierTransform.GraphControl();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.SuspendLayout();
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.flowLayoutPanel1.AutoScroll = true;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(7, 29);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(390, 400);
            this.flowLayoutPanel1.TabIndex = 3;
            this.flowLayoutPanel1.WrapContents = false;
            // 
            // addWaveButton
            // 
            this.addWaveButton.Location = new System.Drawing.Point(24, 44);
            this.addWaveButton.Name = "addWaveButton";
            this.addWaveButton.Size = new System.Drawing.Size(75, 20);
            this.addWaveButton.TabIndex = 4;
            this.addWaveButton.Text = "Add Wave";
            this.addWaveButton.UseVisualStyleBackColor = true;
            this.addWaveButton.Click += new System.EventHandler(this.addWaveButton_Click);
            // 
            // compositeVisibleCheckbox
            // 
            this.compositeVisibleCheckbox.AutoSize = true;
            this.compositeVisibleCheckbox.Checked = true;
            this.compositeVisibleCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.compositeVisibleCheckbox.Location = new System.Drawing.Point(421, 47);
            this.compositeVisibleCheckbox.Name = "compositeVisibleCheckbox";
            this.compositeVisibleCheckbox.Size = new System.Drawing.Size(108, 17);
            this.compositeVisibleCheckbox.TabIndex = 5;
            this.compositeVisibleCheckbox.Text = "Composite Visible";
            this.compositeVisibleCheckbox.UseVisualStyleBackColor = true;
            this.compositeVisibleCheckbox.CheckedChanged += new System.EventHandler(this.compositeVisibleCheckbox_CheckedChanged);
            // 
            // freqDomainRadioButton
            // 
            this.freqDomainRadioButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.freqDomainRadioButton.AutoSize = true;
            this.freqDomainRadioButton.Location = new System.Drawing.Point(713, 46);
            this.freqDomainRadioButton.Name = "freqDomainRadioButton";
            this.freqDomainRadioButton.Size = new System.Drawing.Size(114, 17);
            this.freqDomainRadioButton.TabIndex = 6;
            this.freqDomainRadioButton.Text = "Frequency Domain";
            this.freqDomainRadioButton.UseVisualStyleBackColor = true;
            this.freqDomainRadioButton.CheckedChanged += new System.EventHandler(this.freqDomainRadioButton_CheckedChanged);
            // 
            // timeDomainRadioButton
            // 
            this.timeDomainRadioButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.timeDomainRadioButton.AutoSize = true;
            this.timeDomainRadioButton.Checked = true;
            this.timeDomainRadioButton.Location = new System.Drawing.Point(611, 46);
            this.timeDomainRadioButton.Name = "timeDomainRadioButton";
            this.timeDomainRadioButton.Size = new System.Drawing.Size(87, 17);
            this.timeDomainRadioButton.TabIndex = 7;
            this.timeDomainRadioButton.TabStop = true;
            this.timeDomainRadioButton.Text = "Time Domain";
            this.timeDomainRadioButton.UseVisualStyleBackColor = true;
            this.timeDomainRadioButton.CheckedChanged += new System.EventHandler(this.timeDomainRadioButton_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.timeDomainRadioButton);
            this.groupBox1.Controls.Add(this.addWaveButton);
            this.groupBox1.Controls.Add(this.compositeVisibleCheckbox);
            this.groupBox1.Controls.Add(this.freqDomainRadioButton);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.groupBox1.Location = new System.Drawing.Point(-15, -36);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(861, 529);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            // 
            // chart1
            // 
            this.chart1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.chart1.BackColor = System.Drawing.Color.Gray;
            this.chart1.BorderlineColor = System.Drawing.Color.Gray;
            chartArea1.AxisX.LabelStyle.Enabled = false;
            chartArea1.AxisX.LineColor = System.Drawing.Color.Orange;
            chartArea1.AxisX.MajorGrid.Enabled = false;
            chartArea1.AxisX.MajorTickMark.Enabled = false;
            chartArea1.AxisY.LabelStyle.Enabled = false;
            chartArea1.AxisY.LineColor = System.Drawing.Color.Orange;
            chartArea1.AxisY.MajorGrid.Enabled = false;
            chartArea1.AxisY.MajorTickMark.Enabled = false;
            chartArea1.BackColor = System.Drawing.Color.Gray;
            chartArea1.BorderColor = System.Drawing.Color.Gray;
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(406, 31);
            this.chart1.Name = "chart1";
            series1.BackSecondaryColor = System.Drawing.Color.White;
            series1.BorderColor = System.Drawing.Color.White;
            series1.BorderWidth = 0;
            series1.ChartArea = "ChartArea1";
            series1.Color = System.Drawing.Color.White;
            series1.IsVisibleInLegend = false;
            series1.LabelForeColor = System.Drawing.Color.White;
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            series1.SmartLabelStyle.Enabled = false;
            series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(400, 400);
            this.chart1.TabIndex = 8;
            this.chart1.Text = "chart1";
            this.chart1.Visible = false;
            // 
            // graphControl1
            // 
            this.graphControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.graphControl1.CompositeVisible = true;
            this.graphControl1.Location = new System.Drawing.Point(406, 31);
            this.graphControl1.Name = "graphControl1";
            this.graphControl1.Size = new System.Drawing.Size(400, 400);
            this.graphControl1.TabIndex = 0;
            this.graphControl1.Text = "graphControl1";
            this.graphControl1.DataChanged += new FourierTransform.GraphControl.DataChangedHandler(this.graphControl1_DataChanged);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(814, 461);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.flowLayoutPanel1);
            this.Controls.Add(this.graphControl1);
            this.Controls.Add(this.groupBox1);
            this.Name = "Main";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Fourier Transform";
            this.Load += new System.EventHandler(this.Main_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private GraphControl graphControl1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button addWaveButton;
        private System.Windows.Forms.CheckBox compositeVisibleCheckbox;
        private System.Windows.Forms.RadioButton freqDomainRadioButton;
        private System.Windows.Forms.RadioButton timeDomainRadioButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
    }
}