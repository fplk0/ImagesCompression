using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab1
{
    public partial class FullImageForm : Form
    {
        public FullImageForm()
        {
            InitializeComponent();
        }

        public void setImage(System.Drawing.Image img)
        {
            if (img == null)
                return;
            imagePictureBox.Image = img;
            imagePictureBox.Width = img.Width;
            imagePictureBox.Height = img.Height;
        }
    }
}
