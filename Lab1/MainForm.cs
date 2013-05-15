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
    public partial class MainForm : Form, ColorChannelControlDelegate, ImageProcessingObserver
    {
        public MainForm()
        {
            InitializeComponent();

            //String path = "C:\\Users\\Sfairat\\Documents\\Visual Studio 2012\\Projects\\University\\ImagesCompression\\Lab13\\Lab13\\Resources\\maelstromBig.jpg";
            //CLRWrapper.Lab1Wrapper.decodeJPEGStatic(path);

            this.initializeColorSpaces();

            foreach (ColorSpace cs in this.colorSpaces)
            {
                colorSpacePickerComboBox.Items.Add(cs.name);
            }

            myTimer = new Timer();
            myTimer.Interval = 1000 / 30;
            myTimer.Tick += timerHandler;

            colorSpacePickerComboBox.SelectedIndex = 0;

            filters = KnownFilters.get();

            int filtersHeight = 0;
            int filtersTopOffset = 15;
            filterCheckBoxes = new CheckBox[filters.Length];

            for (int i = 0; i < filters.Length; i++)
            {
                int sideOffsets = 5;
                CheckBox filterCheckBox = new CheckBox();
                filterCheckBox.Checked = false;
                filterCheckBox.Text = filters[i].name;
                filterCheckBox.Left = sideOffsets;
                filterCheckBox.Width = filterGroupBox.Width - sideOffsets * 2;
                filterCheckBox.Top = filtersHeight + filtersTopOffset;
                filterCheckBox.Height = 20;
                filtersHeight += filterCheckBox.Height;
                filterGroupBox.Controls.Add(filterCheckBox);
                filterCheckBoxes[i] = filterCheckBox;
                filterCheckBox.CheckedChanged += filterCheckBox_CheckedChanged;
            }

            filterGroupBox.Height = filtersHeight + filtersTopOffset + 5;
        }

        void filterCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            this.controlsChanged();
        }

        System.Drawing.Image originalImage;
        System.Drawing.Image modifiedImage;
        Image myModifiedImage;
        ImageProcessing currentImageProcessing;
        ColorChannelControl allChannelsControl;
        ColorChannelControl[] separateChannelControls;
        Filter[] filters;
        CheckBox[] filterCheckBoxes;
        FullImageForm fiForm;
        FullImageForm originalFIForm;

        void timerHandler(object sender, EventArgs e)
        {
            myTimer.Enabled = false;
            myTimer.Stop();

            if (originalImage == null)
                return;

            //ImageProcessingManager.singleInstance.Ca

            ColorSpace cs = colorSpaces[colorSpacePickerComboBox.SelectedIndex];
            ImageProcessing imgProc = new ImageProcessing(originalImage, cs);
            currentImageProcessing = imgProc;

            for (int i = 0; i < cs.componentsCount; i++)
            {
                ColorChannelControl currentChannelControl = separateChannelControls[i];
                currentImageProcessing.addedBrightness[i] = addForControlValue(allChannelsControl.addedBrightness) + addForControlValue(currentChannelControl.addedBrightness);
                currentImageProcessing.multipliedBrightness[i] = multiplierForControlValue(allChannelsControl.multipliedBrightness) * multiplierForControlValue(currentChannelControl.multipliedBrightness);
                currentImageProcessing.selectedComponents[i] = currentChannelControl.channelEnabled;
                currentImageProcessing.componentsGamma[i] = allChannelsControl.gamma * currentChannelControl.gamma;
            }

            List<Filter> filtersToApply = new List<Filter>();
            for (int i = 0; i < filters.Length; i++)
            {
                if (filterCheckBoxes[i].Checked)
                {
                    filtersToApply.Add(filters[i]);
                }
            }
            imgProc.filtersToApply = filtersToApply;

            imgProc.useDWT = this.freqRadioButtonDWT.Checked;
            imgProc.dwtBlockSize = (int)this.dwtSizeNumericUpDown.Value;
            imgProc.dwtDepth = (int)this.dwtDepthNumericUpDown.Value;
            imgProc.useDWTQuant = this.dwtQuantizationCheckBox.Checked;
            imgProc.dwtQuantQuality = (int)this.dwtQuantizationQuality.Value;

            imgProc.useDCT = this.FreqRadioButtonDCT.Checked;
            imgProc.dctBlockSize = (int)this.dctBlockSizeNumericUpDown.Value;
            imgProc.dctZeroCoef = (int)this.dctZeroCoefNumericUpDown.Value;
            imgProc.useDCTQuant = this.dctUseQuantizationCheckBox.Checked;
            imgProc.dctQuantQuality = (int)this.dctQuantizationQualityNumericUpDown.Value;

            imgProc.addObserver(this);

            ImageProcessingManager.Instance.startImageProcessing(imgProc);
        }

        float multiplierForControlValue(int controlValue)
        {
            bool negative = false;
            if (controlValue < 0)
            {
                negative = true;
                controlValue = -controlValue;
            }

            float retVal = 3 * controlValue / 100.0f + 1;
            if (negative)
                retVal = 1 / retVal;
            return retVal;
        }

        float addForControlValue(int controlValue)
        {
            return controlValue / 100.0f;
        }


        public Timer myTimer;

        void initializeColorSpaces()
        {
            colorSpaces = new List<ColorSpace>();
            colorSpaces.Add(new ColorSpaceRGB());
            colorSpaces.Add(new ColorSpaceYCbCr());
            colorSpaces.Add(new ColorSpaceGS());
        }

        private List<ColorSpace> colorSpaces;

        private void openImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.FileOk += fileOkHandler;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string[] splitted = openFileDialog.FileName.Split('.');
                string fileExt = splitted[splitted.Length - 1];
                if (fileExt == "jpg" || fileExt == "jpeg")
                {
                    float[, ,] pixels = CLRWrapper.Lab1Wrapper.decodeJPEGStatic(openFileDialog.FileName);
                    Image img = new Image(pixels, new ColorSpaceRGB());
                    originalImage = img.convertToBitmap();
                }
                else
                {
                    try
                    {
                        originalImage = System.Drawing.Image.FromFile(openFileDialog.FileName);
                    }
                    catch (Exception)
                    {
                        MessageBox.Show("Image is invalid");
                    }
                }

                originalImageBox.Image = originalImage;

                if (originalFIForm != null)
                    originalFIForm.setImage(originalImage);

                this.controlsChanged();
            }
        }

        private void fileOkHandler(object sender, CancelEventArgs e)
        {
            OpenFileDialog fileDialog = (OpenFileDialog)sender;
            if (!System.IO.File.Exists(fileDialog.FileName))
                e.Cancel = true;
        }

        private void flowLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        void controlsChanged()
        {
            if (!myTimer.Enabled)
            {
                myTimer.Start();
                myTimer.Enabled = true;
            }
        }

        #region ColorChannelControlDelegate

        public void colorChannelControlValueChanged(ColorChannelControl control)
        {
            this.controlsChanged();
        }

        #endregion

        #region ImageProcessingDelegate

        public void didFinishImageProcessing(ImageProcessing imageProcessing)
        {
            Image resultImage = imageProcessing.resultImage;

            System.Drawing.Bitmap bmp = resultImage.convertToBitmap();

            Func<int> handler = () =>
            {
                myModifiedImage = resultImage;
                modifiedImage = bmp;
                modifiedImageBox.Image = modifiedImage;
                if (fiForm != null)
                    fiForm.setImage(modifiedImage);
                return 0;
            };
            this.BeginInvoke(handler);
            if (currentImageProcessing == imageProcessing)
                currentImageProcessing = null;

            imageProcessing.removeObserver(this);
        }

        public void didCancelImageProcessing(ImageProcessing imageProcessing)
        {
            imageProcessing.removeObserver(this);
        }

        public void imageProcessingDidFinish(ImageProcessing imageProcessingOperation, Image resultImage)
        {
            System.Drawing.Bitmap bmp = resultImage.convertToBitmap();
            Func<int> handler = () => {
                myModifiedImage = resultImage;
                modifiedImage = bmp;
                //modifiedImageBox.Image = modifiedImage;
                if (fiForm != null)
                    fiForm.setImage(modifiedImage);
                return 0; 
            };
            this.BeginInvoke(handler);
            if (currentImageProcessing == imageProcessingOperation)
                currentImageProcessing = null;
            //setModifiedImage;
        }

        #endregion

        private void colorSpacePickerComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            refreshColorChannels();
            controlsChanged();
        }

        private void refreshColorChannels()
        {
            channelsGroupBox.Controls.Clear();
            ColorSpace cs = colorSpaces[colorSpacePickerComboBox.SelectedIndex];

            int totalHeight = 15;
            int left = 2;
            allChannelsControl = new ColorChannelControl();
            allChannelsControl.hasChannelDisabler = false;
            allChannelsControl.channelName = "All";
            allChannelsControl.Top = totalHeight;
            allChannelsControl.Left = left;
            allChannelsControl.classDelegate = this;
            totalHeight += allChannelsControl.Size.Height;
            channelsGroupBox.Controls.Add(allChannelsControl);

            separateChannelControls = new ColorChannelControl[cs.componentsCount];
            for (int i = 0, iEnd = cs.componentsCount; i < iEnd; i++)
            {
                ColorChannelControl control = new ColorChannelControl();
                control.hasChannelDisabler = true;
                separateChannelControls[i] = control;
                control.Top = totalHeight;
                control.Left = left;
                control.channelName = cs.nameOfComponentAtIndex(i);
                control.classDelegate = this;
                channelsGroupBox.Controls.Add(control);
                totalHeight += control.Size.Height;
            }

            channelsGroupBox.Height = totalHeight + 15;
        }

        void showFullModifiedImage()
        {
            if (fiForm == null)
            {
                fiForm = new FullImageForm();
                fiForm.setImage(modifiedImage);
                fiForm.Show();
                fiForm.FormClosed += fiForm_FormClosed;
            }
            else
                fiForm.BringToFront();
        }

        private void fullImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showFullModifiedImage();
        }

        void fiForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (fiForm == (FullImageForm)sender)
                fiForm = null;
            else if (originalFIForm == (FullImageForm)sender)
                originalFIForm = null;
        }

        private void modifiedImageBox_Click(object sender, EventArgs e)
        {
            showFullModifiedImage();
        }

        void showFullOriginalImage()
        {
            if (originalFIForm == null)
            {
                originalFIForm = new FullImageForm();
                originalFIForm.setImage(originalImage);
                originalFIForm.Show();
                originalFIForm.FormClosed += fiForm_FormClosed;
            }
            else
                originalFIForm.BringToFront();
        }

        private void originalImageBox_Click(object sender, EventArgs e)
        {
            showFullOriginalImage();
        }

        private void controlValueChanged(object sender, EventArgs e)
        {
            this.controlsChanged();
        }

        private void numericUpDown1_Enter(object sender, EventArgs e)
        {

        }

        private void statisticsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            (new StatisticsForm()).Show();
        }

        private void saveImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConvertImageForm form = new ConvertImageForm();
            form.imageToConvert = myModifiedImage;

            form.Show();
        }
    }
}
