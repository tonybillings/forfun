namespace RobotMapper
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
            this.chkDrawMarkers = new System.Windows.Forms.CheckBox();
            this.chkDrawLines = new System.Windows.Forms.CheckBox();
            this.chkDrawGrid = new System.Windows.Forms.CheckBox();
            this.chkDrawRobot = new System.Windows.Forms.CheckBox();
            this.chkDrawPath = new System.Windows.Forms.CheckBox();
            this.sliderMarkerConnectionRadius = new System.Windows.Forms.TrackBar();
            this.lblMarkerConnectionRadius = new System.Windows.Forms.Label();
            this.xnaControl = new RobotMapper.MainGraphicsDeviceControl();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.sliderMarkerConnectionRadius)).BeginInit();
            this.SuspendLayout();
            // 
            // chkDrawMarkers
            // 
            this.chkDrawMarkers.AutoSize = true;
            this.chkDrawMarkers.BackColor = System.Drawing.Color.Transparent;
            this.chkDrawMarkers.Checked = true;
            this.chkDrawMarkers.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDrawMarkers.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkDrawMarkers.ForeColor = System.Drawing.Color.White;
            this.chkDrawMarkers.Location = new System.Drawing.Point(7, 42);
            this.chkDrawMarkers.Name = "chkDrawMarkers";
            this.chkDrawMarkers.Size = new System.Drawing.Size(104, 17);
            this.chkDrawMarkers.TabIndex = 5;
            this.chkDrawMarkers.TabStop = false;
            this.chkDrawMarkers.Text = "Draw Markers";
            this.chkDrawMarkers.UseVisualStyleBackColor = false;
            this.chkDrawMarkers.CheckedChanged += new System.EventHandler(this.chkDrawMarkers_CheckedChanged);
            // 
            // chkDrawLines
            // 
            this.chkDrawLines.AutoSize = true;
            this.chkDrawLines.BackColor = System.Drawing.Color.Transparent;
            this.chkDrawLines.Checked = true;
            this.chkDrawLines.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDrawLines.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkDrawLines.ForeColor = System.Drawing.Color.White;
            this.chkDrawLines.Location = new System.Drawing.Point(7, 25);
            this.chkDrawLines.Name = "chkDrawLines";
            this.chkDrawLines.Size = new System.Drawing.Size(89, 17);
            this.chkDrawLines.TabIndex = 6;
            this.chkDrawLines.TabStop = false;
            this.chkDrawLines.Text = "Draw Lines";
            this.chkDrawLines.UseVisualStyleBackColor = false;
            this.chkDrawLines.CheckedChanged += new System.EventHandler(this.chkDrawLines_CheckedChanged);
            // 
            // chkDrawGrid
            // 
            this.chkDrawGrid.AutoSize = true;
            this.chkDrawGrid.BackColor = System.Drawing.Color.Transparent;
            this.chkDrawGrid.Checked = true;
            this.chkDrawGrid.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDrawGrid.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkDrawGrid.ForeColor = System.Drawing.Color.White;
            this.chkDrawGrid.Location = new System.Drawing.Point(7, 8);
            this.chkDrawGrid.Name = "chkDrawGrid";
            this.chkDrawGrid.Size = new System.Drawing.Size(82, 17);
            this.chkDrawGrid.TabIndex = 7;
            this.chkDrawGrid.TabStop = false;
            this.chkDrawGrid.Text = "Draw Grid";
            this.chkDrawGrid.UseVisualStyleBackColor = false;
            this.chkDrawGrid.CheckedChanged += new System.EventHandler(this.chkDrawGrid_CheckedChanged);
            // 
            // chkDrawRobot
            // 
            this.chkDrawRobot.AutoSize = true;
            this.chkDrawRobot.BackColor = System.Drawing.Color.Transparent;
            this.chkDrawRobot.Checked = true;
            this.chkDrawRobot.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDrawRobot.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkDrawRobot.ForeColor = System.Drawing.Color.White;
            this.chkDrawRobot.Location = new System.Drawing.Point(7, 59);
            this.chkDrawRobot.Name = "chkDrawRobot";
            this.chkDrawRobot.Size = new System.Drawing.Size(93, 17);
            this.chkDrawRobot.TabIndex = 8;
            this.chkDrawRobot.TabStop = false;
            this.chkDrawRobot.Text = "Draw Robot";
            this.chkDrawRobot.UseVisualStyleBackColor = false;
            this.chkDrawRobot.CheckedChanged += new System.EventHandler(this.chkDrawRobot_CheckedChanged);
            // 
            // chkDrawPath
            // 
            this.chkDrawPath.AutoSize = true;
            this.chkDrawPath.BackColor = System.Drawing.Color.Transparent;
            this.chkDrawPath.Checked = true;
            this.chkDrawPath.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDrawPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkDrawPath.ForeColor = System.Drawing.Color.White;
            this.chkDrawPath.Location = new System.Drawing.Point(7, 76);
            this.chkDrawPath.Name = "chkDrawPath";
            this.chkDrawPath.Size = new System.Drawing.Size(85, 17);
            this.chkDrawPath.TabIndex = 9;
            this.chkDrawPath.TabStop = false;
            this.chkDrawPath.Text = "Draw Path";
            this.chkDrawPath.UseVisualStyleBackColor = false;
            this.chkDrawPath.CheckedChanged += new System.EventHandler(this.chkDrawPath_CheckedChanged);
            // 
            // sliderMarkerConnectionRadius
            // 
            this.sliderMarkerConnectionRadius.AutoSize = false;
            this.sliderMarkerConnectionRadius.LargeChange = 1;
            this.sliderMarkerConnectionRadius.Location = new System.Drawing.Point(642, 21);
            this.sliderMarkerConnectionRadius.Maximum = 100;
            this.sliderMarkerConnectionRadius.Minimum = 1;
            this.sliderMarkerConnectionRadius.Name = "sliderMarkerConnectionRadius";
            this.sliderMarkerConnectionRadius.Size = new System.Drawing.Size(154, 27);
            this.sliderMarkerConnectionRadius.TabIndex = 10;
            this.sliderMarkerConnectionRadius.TabStop = false;
            this.sliderMarkerConnectionRadius.TickStyle = System.Windows.Forms.TickStyle.None;
            this.sliderMarkerConnectionRadius.Value = 20;
            this.sliderMarkerConnectionRadius.Scroll += new System.EventHandler(this.sliderMarkerConnectionRadius_Scroll);
            // 
            // lblMarkerConnectionRadius
            // 
            this.lblMarkerConnectionRadius.AutoSize = true;
            this.lblMarkerConnectionRadius.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMarkerConnectionRadius.ForeColor = System.Drawing.Color.White;
            this.lblMarkerConnectionRadius.Location = new System.Drawing.Point(639, 8);
            this.lblMarkerConnectionRadius.Name = "lblMarkerConnectionRadius";
            this.lblMarkerConnectionRadius.Size = new System.Drawing.Size(157, 13);
            this.lblMarkerConnectionRadius.TabIndex = 11;
            this.lblMarkerConnectionRadius.Text = "Marker Connection Radius";
            // 
            // xnaControl
            // 
            this.xnaControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.xnaControl.Location = new System.Drawing.Point(0, 0);
            this.xnaControl.MarkerConnectionRadius = 0;
            this.xnaControl.Name = "xnaControl";
            this.xnaControl.Size = new System.Drawing.Size(800, 800);
            this.xnaControl.TabIndex = 4;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(364, 42);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 12;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 800);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.lblMarkerConnectionRadius);
            this.Controls.Add(this.sliderMarkerConnectionRadius);
            this.Controls.Add(this.chkDrawPath);
            this.Controls.Add(this.chkDrawRobot);
            this.Controls.Add(this.chkDrawGrid);
            this.Controls.Add(this.chkDrawLines);
            this.Controls.Add(this.chkDrawMarkers);
            this.Controls.Add(this.xnaControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "Main";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Robot Mapper";
            this.TopMost = true;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Main_FormClosing);
            this.Load += new System.EventHandler(this.Main_Load);
            ((System.ComponentModel.ISupportInitialize)(this.sliderMarkerConnectionRadius)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MainGraphicsDeviceControl xnaControl;
        private System.Windows.Forms.CheckBox chkDrawMarkers;
        private System.Windows.Forms.CheckBox chkDrawLines;
        private System.Windows.Forms.CheckBox chkDrawGrid;
        private System.Windows.Forms.CheckBox chkDrawRobot;
        private System.Windows.Forms.CheckBox chkDrawPath;
        private System.Windows.Forms.TrackBar sliderMarkerConnectionRadius;
        private System.Windows.Forms.Label lblMarkerConnectionRadius;
        private System.Windows.Forms.Button button1;
        

    }
}