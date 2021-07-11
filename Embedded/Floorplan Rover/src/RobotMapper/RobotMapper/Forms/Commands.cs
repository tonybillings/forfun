using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RobotMapper
{
    public partial class Commands : Form
    {
        private Dictionary<String, Image> _images;

        public Commands()
        {
            InitializeComponent();
        }

        private void Commands_Load(object sender, EventArgs e)
        {
            InitializeImages();
            SetBackgroundImage("DEFAULT");
        }

        private void InitializeImages()
        {
            _images = new Dictionary<String, Image>();
            _images.Add("DEFAULT", Properties.Resources.RoverCommandsDefault);
            _images.Add("STOP", Properties.Resources.RoverCommandsStop);
            _images.Add("AUTO", Properties.Resources.RoverCommandsAuto);
            _images.Add("LEFT", Properties.Resources.RoverCommandsLeft);
            _images.Add("FORWARD", Properties.Resources.RoverCommandsForward);
            _images.Add("RIGHT", Properties.Resources.RoverCommandsRight);
            _images.Add("REVERSE", Properties.Resources.RoverCommandsReverse);
            _images.Add("STAR", Properties.Resources.RoverCommandsStar);
            _images.Add("SQUARE", Properties.Resources.RoverCommandsSquare);
        }

        void SetBackgroundImage(string name)
        {
            Image img;
            _images.TryGetValue(name, out img);
            this.BackgroundImage = img;
        }

        private void mouseCatcherLabel_MouseEnter(object sender, EventArgs e)
        {
            Label lbl = sender as Label;
            SetBackgroundImage(lbl.Tag.ToString().ToUpper());
        }

        private void mouseCatcherLabel_MouseLeave(object sender, EventArgs e)
        {
            SetBackgroundImage("DEFAULT");
        }

    
       
    }
}
