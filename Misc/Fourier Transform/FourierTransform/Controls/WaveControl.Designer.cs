namespace FourierTransform
{
    partial class WaveControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(WaveControl));
            this.enabledCheckBox = new System.Windows.Forms.CheckBox();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.visibleCheckbox = new System.Windows.Forms.CheckBox();
            this.deleteLabel = new System.Windows.Forms.Label();
            this.colorLabel = new System.Windows.Forms.Label();
            this.slidersCheckbox = new System.Windows.Forms.CheckBox();
            this.trackbarPanel = new System.Windows.Forms.Panel();
            this.phaseLabel = new System.Windows.Forms.Label();
            this.ampLabel = new System.Windows.Forms.Label();
            this.freqLabel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.phaseTrackBar = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.ampTrackBar = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.freqTrackBar = new System.Windows.Forms.TrackBar();
            this.textboxPanel = new System.Windows.Forms.Panel();
            this.phaseTextbox = new System.Windows.Forms.TextBox();
            this.ampTextbox = new System.Windows.Forms.TextBox();
            this.freqTextbox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.trackbarPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.phaseTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ampTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.freqTrackBar)).BeginInit();
            this.textboxPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // enabledCheckBox
            // 
            this.enabledCheckBox.AutoSize = true;
            this.enabledCheckBox.BackColor = System.Drawing.Color.Transparent;
            this.enabledCheckBox.Checked = true;
            this.enabledCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enabledCheckBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.enabledCheckBox.ForeColor = System.Drawing.Color.White;
            this.enabledCheckBox.Location = new System.Drawing.Point(5, 3);
            this.enabledCheckBox.Name = "enabledCheckBox";
            this.enabledCheckBox.Size = new System.Drawing.Size(72, 17);
            this.enabledCheckBox.TabIndex = 13;
            this.enabledCheckBox.Text = "Enabled";
            this.enabledCheckBox.UseVisualStyleBackColor = false;
            this.enabledCheckBox.CheckedChanged += new System.EventHandler(this.enabledCheckBox_CheckedChanged);
            // 
            // visibleCheckbox
            // 
            this.visibleCheckbox.AutoSize = true;
            this.visibleCheckbox.BackColor = System.Drawing.Color.Transparent;
            this.visibleCheckbox.Checked = true;
            this.visibleCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.visibleCheckbox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.visibleCheckbox.ForeColor = System.Drawing.Color.White;
            this.visibleCheckbox.Location = new System.Drawing.Point(83, 3);
            this.visibleCheckbox.Name = "visibleCheckbox";
            this.visibleCheckbox.Size = new System.Drawing.Size(63, 17);
            this.visibleCheckbox.TabIndex = 19;
            this.visibleCheckbox.Text = "Visible";
            this.visibleCheckbox.UseVisualStyleBackColor = false;
            this.visibleCheckbox.CheckedChanged += new System.EventHandler(this.visibleCheckbox_CheckedChanged);
            // 
            // deleteLabel
            // 
            this.deleteLabel.BackColor = System.Drawing.Color.Transparent;
            this.deleteLabel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.deleteLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.deleteLabel.ForeColor = System.Drawing.Color.Red;
            this.deleteLabel.Image = ((System.Drawing.Image)(resources.GetObject("deleteLabel.Image")));
            this.deleteLabel.Location = new System.Drawing.Point(333, 3);
            this.deleteLabel.Name = "deleteLabel";
            this.deleteLabel.Size = new System.Drawing.Size(25, 20);
            this.deleteLabel.TabIndex = 20;
            this.deleteLabel.Click += new System.EventHandler(this.deleteLabel_Click);
            // 
            // colorLabel
            // 
            this.colorLabel.BackColor = System.Drawing.Color.Transparent;
            this.colorLabel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.colorLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.colorLabel.ForeColor = System.Drawing.Color.Red;
            this.colorLabel.Image = ((System.Drawing.Image)(resources.GetObject("colorLabel.Image")));
            this.colorLabel.Location = new System.Drawing.Point(309, 3);
            this.colorLabel.Name = "colorLabel";
            this.colorLabel.Size = new System.Drawing.Size(25, 20);
            this.colorLabel.TabIndex = 14;
            this.colorLabel.Click += new System.EventHandler(this.colorLabel_Click);
            // 
            // slidersCheckbox
            // 
            this.slidersCheckbox.AutoSize = true;
            this.slidersCheckbox.BackColor = System.Drawing.Color.Transparent;
            this.slidersCheckbox.Checked = true;
            this.slidersCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.slidersCheckbox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.slidersCheckbox.ForeColor = System.Drawing.Color.White;
            this.slidersCheckbox.Location = new System.Drawing.Point(152, 3);
            this.slidersCheckbox.Name = "slidersCheckbox";
            this.slidersCheckbox.Size = new System.Drawing.Size(64, 17);
            this.slidersCheckbox.TabIndex = 21;
            this.slidersCheckbox.Text = "Sliders";
            this.slidersCheckbox.UseVisualStyleBackColor = false;
            this.slidersCheckbox.CheckedChanged += new System.EventHandler(this.slidersCheckbox_CheckedChanged);
            // 
            // trackbarPanel
            // 
            this.trackbarPanel.Controls.Add(this.phaseLabel);
            this.trackbarPanel.Controls.Add(this.ampLabel);
            this.trackbarPanel.Controls.Add(this.freqLabel);
            this.trackbarPanel.Controls.Add(this.label3);
            this.trackbarPanel.Controls.Add(this.phaseTrackBar);
            this.trackbarPanel.Controls.Add(this.label2);
            this.trackbarPanel.Controls.Add(this.ampTrackBar);
            this.trackbarPanel.Controls.Add(this.label1);
            this.trackbarPanel.Controls.Add(this.freqTrackBar);
            this.trackbarPanel.Location = new System.Drawing.Point(3, 26);
            this.trackbarPanel.Name = "trackbarPanel";
            this.trackbarPanel.Size = new System.Drawing.Size(353, 164);
            this.trackbarPanel.TabIndex = 22;
            // 
            // phaseLabel
            // 
            this.phaseLabel.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.phaseLabel.ForeColor = System.Drawing.Color.White;
            this.phaseLabel.Location = new System.Drawing.Point(196, 137);
            this.phaseLabel.Name = "phaseLabel";
            this.phaseLabel.Size = new System.Drawing.Size(108, 18);
            this.phaseLabel.TabIndex = 26;
            this.phaseLabel.Text = "(0)";
            // 
            // ampLabel
            // 
            this.ampLabel.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ampLabel.ForeColor = System.Drawing.Color.White;
            this.ampLabel.Location = new System.Drawing.Point(211, 84);
            this.ampLabel.Name = "ampLabel";
            this.ampLabel.Size = new System.Drawing.Size(108, 18);
            this.ampLabel.TabIndex = 25;
            this.ampLabel.Text = "(1)";
            // 
            // freqLabel
            // 
            this.freqLabel.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.freqLabel.ForeColor = System.Drawing.Color.White;
            this.freqLabel.Location = new System.Drawing.Point(210, 29);
            this.freqLabel.Name = "freqLabel";
            this.freqLabel.Size = new System.Drawing.Size(108, 18);
            this.freqLabel.TabIndex = 24;
            this.freqLabel.Text = "(1)";
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(0, 137);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(353, 18);
            this.label3.TabIndex = 23;
            this.label3.Text = "phase";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // phaseTrackBar
            // 
            this.phaseTrackBar.LargeChange = 1;
            this.phaseTrackBar.Location = new System.Drawing.Point(0, 117);
            this.phaseTrackBar.Name = "phaseTrackBar";
            this.phaseTrackBar.Size = new System.Drawing.Size(353, 45);
            this.phaseTrackBar.TabIndex = 22;
            this.phaseTrackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.phaseTrackBar.ValueChanged += new System.EventHandler(this.phaseTrackBar_ValueChanged);
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(0, 84);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(353, 18);
            this.label2.TabIndex = 21;
            this.label2.Text = "amplitude";
            this.label2.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // ampTrackBar
            // 
            this.ampTrackBar.LargeChange = 1;
            this.ampTrackBar.Location = new System.Drawing.Point(0, 64);
            this.ampTrackBar.Maximum = 20;
            this.ampTrackBar.Name = "ampTrackBar";
            this.ampTrackBar.Size = new System.Drawing.Size(353, 45);
            this.ampTrackBar.TabIndex = 20;
            this.ampTrackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.ampTrackBar.Value = 10;
            this.ampTrackBar.ValueChanged += new System.EventHandler(this.ampTrackBar_ValueChanged);
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(1, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(350, 18);
            this.label1.TabIndex = 19;
            this.label1.Text = "frequency";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // freqTrackBar
            // 
            this.freqTrackBar.LargeChange = 1;
            this.freqTrackBar.Location = new System.Drawing.Point(0, 9);
            this.freqTrackBar.Maximum = 20;
            this.freqTrackBar.Name = "freqTrackBar";
            this.freqTrackBar.Size = new System.Drawing.Size(353, 45);
            this.freqTrackBar.TabIndex = 18;
            this.freqTrackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.freqTrackBar.Value = 1;
            this.freqTrackBar.ValueChanged += new System.EventHandler(this.freqTrackBar_ValueChanged);
            // 
            // textboxPanel
            // 
            this.textboxPanel.Controls.Add(this.phaseTextbox);
            this.textboxPanel.Controls.Add(this.ampTextbox);
            this.textboxPanel.Controls.Add(this.freqTextbox);
            this.textboxPanel.Controls.Add(this.label6);
            this.textboxPanel.Controls.Add(this.label5);
            this.textboxPanel.Controls.Add(this.label4);
            this.textboxPanel.Location = new System.Drawing.Point(3, 26);
            this.textboxPanel.Name = "textboxPanel";
            this.textboxPanel.Size = new System.Drawing.Size(353, 164);
            this.textboxPanel.TabIndex = 23;
            this.textboxPanel.Visible = false;
            // 
            // phaseTextbox
            // 
            this.phaseTextbox.Location = new System.Drawing.Point(124, 117);
            this.phaseTextbox.Name = "phaseTextbox";
            this.phaseTextbox.Size = new System.Drawing.Size(100, 20);
            this.phaseTextbox.TabIndex = 26;
            this.phaseTextbox.Text = "0";
            this.phaseTextbox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.phaseTextbox.TextChanged += new System.EventHandler(this.phaseTextbox_TextChanged);
            // 
            // ampTextbox
            // 
            this.ampTextbox.Location = new System.Drawing.Point(124, 64);
            this.ampTextbox.Name = "ampTextbox";
            this.ampTextbox.Size = new System.Drawing.Size(100, 20);
            this.ampTextbox.TabIndex = 25;
            this.ampTextbox.Text = "1";
            this.ampTextbox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.ampTextbox.TextChanged += new System.EventHandler(this.ampTextbox_TextChanged);
            // 
            // freqTextbox
            // 
            this.freqTextbox.Location = new System.Drawing.Point(124, 9);
            this.freqTextbox.Name = "freqTextbox";
            this.freqTextbox.Size = new System.Drawing.Size(100, 20);
            this.freqTextbox.TabIndex = 24;
            this.freqTextbox.Text = "1";
            this.freqTextbox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.freqTextbox.TextChanged += new System.EventHandler(this.freqTextbox_TextChanged);
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.White;
            this.label6.Location = new System.Drawing.Point(0, 137);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(353, 18);
            this.label6.TabIndex = 24;
            this.label6.Text = "phase";
            this.label6.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(0, 29);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(353, 18);
            this.label5.TabIndex = 23;
            this.label5.Text = "frequency";
            this.label5.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Adobe Fan Heiti Std B", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(0, 84);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(353, 18);
            this.label4.TabIndex = 22;
            this.label4.Text = "amplitude";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // WaveControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.DodgerBlue;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.trackbarPanel);
            this.Controls.Add(this.slidersCheckbox);
            this.Controls.Add(this.deleteLabel);
            this.Controls.Add(this.visibleCheckbox);
            this.Controls.Add(this.colorLabel);
            this.Controls.Add(this.enabledCheckBox);
            this.Controls.Add(this.textboxPanel);
            this.ForeColor = System.Drawing.Color.White;
            this.Name = "WaveControl";
            this.Size = new System.Drawing.Size(361, 193);
            this.trackbarPanel.ResumeLayout(false);
            this.trackbarPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.phaseTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ampTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.freqTrackBar)).EndInit();
            this.textboxPanel.ResumeLayout(false);
            this.textboxPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox enabledCheckBox;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.Label colorLabel;
        private System.Windows.Forms.CheckBox visibleCheckbox;
        private System.Windows.Forms.Label deleteLabel;
        private System.Windows.Forms.CheckBox slidersCheckbox;
        private System.Windows.Forms.Panel trackbarPanel;
        private System.Windows.Forms.Label phaseLabel;
        private System.Windows.Forms.Label ampLabel;
        private System.Windows.Forms.Label freqLabel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar phaseTrackBar;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar ampTrackBar;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar freqTrackBar;
        private System.Windows.Forms.Panel textboxPanel;
        private System.Windows.Forms.TextBox phaseTextbox;
        private System.Windows.Forms.TextBox ampTextbox;
        private System.Windows.Forms.TextBox freqTextbox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
    }
}
