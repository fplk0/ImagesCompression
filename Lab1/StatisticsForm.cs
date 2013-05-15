using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Diagnostics;

using System.IO;

using System.Collections;

using CLRWrapper;

namespace Lab1
{
    public partial class StatisticsForm : Form
    {
        public StatisticsForm()
        {
            InitializeComponent();
        }

        private void runButton_Click(object sender, EventArgs e)
        {
            string folderPath = pathTextBox.Text;
            string[] fileNames = Directory.GetFiles(pathTextBox.Text);

            ArrayList imagesList = new ArrayList(), namesList = new ArrayList();

            for (int i = 0; i < fileNames.Length; i++)
            {
                string cfn = fileNames[i];
                string[] splitted = cfn.Split('\\');
                cfn = splitted[splitted.Length - 1];
                splitted = cfn.Split('/');
                cfn = splitted[splitted.Length - 1];
                splitted = cfn.Split('.');

                string lpc = splitted[splitted.Length - 1];

                if ((lpc == "jpg" || lpc == "jpeg" || lpc == "bmp") && splitted.Length > 1)
                {
                    string fullPath = folderPath + "/" + cfn;
                    try
                    {
                        System.Drawing.Image img = System.Drawing.Image.FromFile(fullPath);
                        Image myImg = new Image(img);
                        imagesList.Add(myImg);

                        cfn = cfn.Remove(cfn.Length - lpc.Length - 1);
                        namesList.Add(cfn);
                    }
                    catch (Exception )
                    {
                        MessageBox.Show("Image is invalid");
                    }
                }
            }

            ArrayList jpegSettings = prepareJPEGSettings();

            statGridView.Rows.Clear();

            string[ , , ] tableValues = new string[jpegSettings.Count, imagesList.Count + 3, 6];

            ParallelOptions opts = new ParallelOptions();
            opts.MaxDegreeOfParallelism = Convert.ToInt32(maxConcurrency.Value);

            Stopwatch totalStopWatch = new Stopwatch();
            totalStopWatch.Start();

            Parallel.For(0, jpegSettings.Count, opts, si =>
            {
                JPEGSettings curSettings = (JPEGSettings)jpegSettings[si];

                //double minEncodeTime = 1e9, maxEncodeTime = 0, avgEncodeTime = 0;
                //double minDecodeTime = 1e9, maxDecodeTime = 0, avgDecodeTime = 0;
                //double minSymmetry = 1e9, maxSymmetry = 0, avgSymmetry = 0;
                //double minCompRatio = 1e9, maxCompRatio = 0, avgCompRatio = 0;

                double[] minValues = { 1e9, 1e9, 1e9, 1e9, 1e9 },
                    maxValues = { 0, 0, 0, 0, 0 },
                    avgValues = { 0, 0, 0, 0, 0 };

                string settingsString = jpegSettingsToString(curSettings);

                string subfolderPath = folderPath + "/" + settingsString;

                Directory.CreateDirectory(subfolderPath);

                for (int i = 0; i < imagesList.Count; i++)
                {
                    Image myImg = (Image)imagesList[i];
                    double[] curValues = { 0, 0, 0, 0, 0 };

                    Stopwatch sw = new Stopwatch();

                    string curImgName = (string)namesList[i];
                    string filePath = subfolderPath + "/" + curImgName + ".jpg";

                    sw.Start();

                    byte[] arr = CLRWrapper.Lab1Wrapper.encodeJPEGStatic(myImg.imagePixels, curSettings);

                    sw.Stop();

                    double curEncodeTime = sw.Elapsed.Milliseconds / 1000.0;
                    curValues[0] = curEncodeTime;

                    File.WriteAllBytes(filePath, arr);

                    sw.Start();

                    //float[,,] pixels = CLRWrapper.Lab1Wrapper.decodeJPEGStatic(filePath);
                    float[, ,] pixels = CLRWrapper.Lab1Wrapper.decodeJPEGStatic(arr);

                    sw.Stop();
                    double curDecodeTime = sw.Elapsed.Milliseconds / 1000.0;
                    curValues[1] = curDecodeTime;

                    int fullSize = myImg.width * myImg.height * 3;
                    int compSize = arr.Length;
                    double curCompRatio = compSize / (double)fullSize;
                    curCompRatio = 1 / curCompRatio;
                    double curSymmetry = curEncodeTime / curDecodeTime;
                    curValues[2] = curSymmetry;
                    curValues[3] = curCompRatio;

                    int h = pixels.GetLength(0), w = pixels.GetLength(1), c = 3;

                    double mse = 0;

                    for (int y = 0; y < h; y++)
                        for (int x = 0; x < w; x++)
                            for (int cc = 0; cc < 3; cc++)
                            {
                                double val = (pixels[y, x, cc] - myImg.imagePixels[y, x, cc]);
                                mse += val * val;
                            }
                    mse = mse / h / w / c;
                    double curPsnr = 10 * Math.Log10(1 / mse);
                    curValues[4] = curPsnr;

                    tableValues[si, i, 0] = curImgName;

                    for (int ind = 0; ind < 5; ind++)
                    {
                        tableValues[si, i, ind + 1] = curValues[ind].ToString();
                        minValues[ind] = Math.Min(minValues[ind], curValues[ind]);
                        maxValues[ind] = Math.Max(maxValues[ind], curValues[ind]);
                        avgValues[ind] += curValues[ind];
                    }
                }

                int tti = imagesList.Count;
                tableValues[si, tti, 0] = "Min";

                for (int ind = 0; ind < 5; ind++)
                {
                    tableValues[si, tti, 1 + ind] = minValues[ind].ToString();
                }


                tti++;
                tableValues[si, tti, 0] = "Max";
                
                for (int ind = 0; ind < 5; ind++)
                {
                    tableValues[si, tti, 1 + ind] = maxValues[ind].ToString();
                }

                tti++;
                tableValues[si, tti, 0] = "Avg";

                for (int ind = 0; ind < 5; ind++)
                {
                    tableValues[si, tti, 1 + ind] = (avgValues[ind] / imagesList.Count).ToString();
                }
            });

            totalStopWatch.Stop();

            testsRunTimeLabel.Text = (totalStopWatch.ElapsedMilliseconds / 1000.0).ToString();

            statGridView.Rows.Add(tableValues.GetLength(0) * (tableValues.GetLength(1) + 1));

            int curRowIndex = 0;

            for (int i = 0; i < jpegSettings.Count; i++)
            {
                JPEGSettings curSettings = (JPEGSettings)jpegSettings[i];
                string settingsString = jpegSettingsToString(curSettings);
                statGridView.Rows[curRowIndex].Cells[0].Value = settingsString;
                curRowIndex++;

                for (int j = 0; j < tableValues.GetLength(1); j++)
                {
                    for (int k = 0; k < tableValues.GetLength(2); k++)
                    {
                        statGridView.Rows[curRowIndex].Cells[k].Value = tableValues[i, j, k];
                    }
                    curRowIndex++;
                }
            }
        }

        string jpegSettingsToString(JPEGSettings settings)
        {
            string retVal = "Q" + settings.quality.ToString() + "h" + settings.horizontalSubsampling[0].ToString() + "v" + settings.verticalSubsampling[0].ToString();
            return retVal;
        }

        ArrayList prepareJPEGSettings()
        {
            ArrayList settings = new ArrayList();

            int[] qualityFactors = { 10, 25, 50, 75, 90, 99, 100 };
            //int[] qualityFactors = { 90 };

            int[] verticalSubsamplingFactorsY = { 2, 1, 1 };
            int[] horizontalSubsamplingFactorsY = { 2, 2, 1 };

            int[] verticalSubsamplingFactorsC = { 1, 1, 1 };
            int[] horizontalSubsamplingFactorsC = { 1, 1, 1 };

            for (int j = 0; j < qualityFactors.Length; j++)
            {
                for (int i = 0; i < verticalSubsamplingFactorsY.Length; i++)
                {
                    JPEGSettings js = new JPEGSettings();
                    js.quality = qualityFactors[j];

                    js.horizontalSubsampling[0] = horizontalSubsamplingFactorsY[i];
                    js.verticalSubsampling[0] = verticalSubsamplingFactorsY[i];
                    js.horizontalSubsampling[1] = js.horizontalSubsampling[2] = 1;
                    js.verticalSubsampling[1] = js.verticalSubsampling[2] = 1;
                    //js.horizontalSubsampling[1] = js.horizontalSubsampling[2] = horizontalSubsamplingFactors[i];
                    //js.verticalSubsampling[1] = js.verticalSubsampling[2] = verticalSubsamplingFactors[i];
                    settings.Add(js);
                }
            }

            return settings;
        }

        private void selectFolderButton_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog dialog = new FolderBrowserDialog())
            {
                dialog.Description = "Open a folder which contains the xml output";
                dialog.ShowNewFolderButton = false;
                dialog.RootFolder = Environment.SpecialFolder.MyComputer;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    string folder = dialog.SelectedPath;
                    pathTextBox.Text = folder;
                }
            }
        }

    }
}
