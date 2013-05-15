using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using CLRWrapper;

namespace Lab1
{
    public partial class ConvertImageForm : Form
    {
        public ConvertImageForm()
        {
            InitializeComponent();
        }

        public Image imageToConvert;

        private void selectFileButton_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog dialog = new SaveFileDialog())
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    string fileName = dialog.FileName;
                    destinationFilePath.Text = fileName;
                }
            }
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            JPEGSettings settings = new JPEGSettings();
            settings.quality = Convert.ToInt32(qualityField.Value);
            settings.horizontalSubsampling[0] = Convert.ToInt32(yHor.Text);
            settings.horizontalSubsampling[1] = Convert.ToInt32(cbHor.Text);
            settings.horizontalSubsampling[2] = Convert.ToInt32(crHor.Text);

            settings.verticalSubsampling[0] = Convert.ToInt32(yVert.Text);
            settings.verticalSubsampling[1] = Convert.ToInt32(cbVert.Text);
            settings.verticalSubsampling[2] = Convert.ToInt32(crVert.Text);

            CLRWrapper.Lab1Wrapper.encodeJPEGStatic(imageToConvert.imagePixels, destinationFilePath.Text, settings);
        }
    }
}
