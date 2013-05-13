using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab1
{
    [ToolboxItem(typeof(ColorChannelControl))]

    public partial class ColorChannelControl : UserControl
    {
        public ColorChannelControl()
        {
            InitializeComponent();
        }

        public ColorChannelControlDelegate classDelegate;

        public string channelName
        {
            set { this.channelNameLabel.Text = value; }
            get { return this.channelNameLabel.Text; }
        }

        public bool channelEnabled
        {
            set { this.selectedCheckBox.Checked = value; }
            get { return this.selectedCheckBox.Checked; }
        }

        public bool hasChannelDisabler
        {
            set { this.selectedCheckBox.Visible = value; }
        }

        public int addedBrightness
        {
            set { this.brightnessAddTrackBar.Value = value; }
            get { return this.brightnessAddTrackBar.Value; }
        }

        public int multipliedBrightness
        {
            set { this.brightnessMultiplyTrackBar.Value = value; }
            get { return this.brightnessMultiplyTrackBar.Value; }
        }

        public float gamma
        {
            set { this.gammaPicker.Value = Convert.ToDecimal(value); }
            get { return (float)Convert.ToDouble(this.gammaPicker.Value); }
        }

        private void controlValueChanged(object sender, EventArgs e)
        {
            if (this.classDelegate != null)
                this.classDelegate.colorChannelControlValueChanged(this);
        }
    }

    public interface ColorChannelControlDelegate
    {
        void colorChannelControlValueChanged(ColorChannelControl control);
    }
}
