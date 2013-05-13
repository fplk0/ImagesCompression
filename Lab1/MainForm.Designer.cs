namespace Lab1
{
    partial class MainForm
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
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fullImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.colorSpaceGroupBox = new System.Windows.Forms.GroupBox();
            this.colorSpacePickerComboBox = new System.Windows.Forms.ComboBox();
            this.channelsGroupBox = new System.Windows.Forms.GroupBox();
            this.filterGroupBox = new System.Windows.Forms.GroupBox();
            this.freqGroupBox = new System.Windows.Forms.GroupBox();
            this.dctQuantizationQualityNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.dctUseQuantizationCheckBox = new System.Windows.Forms.CheckBox();
            this.dctZeroCoefNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.dctBlockSizeNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.dwtQuantizationCheckBox = new System.Windows.Forms.CheckBox();
            this.dwtQuantizationQuality = new System.Windows.Forms.NumericUpDown();
            this.dwtDepthNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.dwtSizeNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.freqRadioButtonDWT = new System.Windows.Forms.RadioButton();
            this.FreqRadioButtonDCT = new System.Windows.Forms.RadioButton();
            this.freqRadioButtonNone = new System.Windows.Forms.RadioButton();
            this.originalImageBox = new System.Windows.Forms.PictureBox();
            this.controlsToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.darkPixelsPercentLabel = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.colorSpaceGroupBox.SuspendLayout();
            this.freqGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dctQuantizationQualityNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dctZeroCoefNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dctBlockSizeNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dwtQuantizationQuality)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dwtDepthNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dwtSizeNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.originalImageBox)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.viewToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1362, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openImageToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openImageToolStripMenuItem
            // 
            this.openImageToolStripMenuItem.Name = "openImageToolStripMenuItem";
            this.openImageToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.openImageToolStripMenuItem.Text = "Open Image...";
            this.openImageToolStripMenuItem.Click += new System.EventHandler(this.openImageToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fullImageToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // fullImageToolStripMenuItem
            // 
            this.fullImageToolStripMenuItem.Name = "fullImageToolStripMenuItem";
            this.fullImageToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.fullImageToolStripMenuItem.Text = "Full Image";
            this.fullImageToolStripMenuItem.Click += new System.EventHandler(this.fullImageToolStripMenuItem_Click);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.flowLayoutPanel1.AutoScroll = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.Controls.Add(this.colorSpaceGroupBox);
            this.flowLayoutPanel1.Controls.Add(this.channelsGroupBox);
            this.flowLayoutPanel1.Controls.Add(this.filterGroupBox);
            this.flowLayoutPanel1.Controls.Add(this.freqGroupBox);
            this.flowLayoutPanel1.Location = new System.Drawing.Point(12, 27);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(320, 702);
            this.flowLayoutPanel1.TabIndex = 1;
            this.flowLayoutPanel1.Paint += new System.Windows.Forms.PaintEventHandler(this.flowLayoutPanel1_Paint);
            // 
            // colorSpaceGroupBox
            // 
            this.colorSpaceGroupBox.Controls.Add(this.colorSpacePickerComboBox);
            this.colorSpaceGroupBox.Location = new System.Drawing.Point(3, 3);
            this.colorSpaceGroupBox.Name = "colorSpaceGroupBox";
            this.colorSpaceGroupBox.Size = new System.Drawing.Size(294, 55);
            this.colorSpaceGroupBox.TabIndex = 0;
            this.colorSpaceGroupBox.TabStop = false;
            this.colorSpaceGroupBox.Text = "Color Space";
            // 
            // colorSpacePickerComboBox
            // 
            this.colorSpacePickerComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.colorSpacePickerComboBox.FormattingEnabled = true;
            this.colorSpacePickerComboBox.Location = new System.Drawing.Point(7, 20);
            this.colorSpacePickerComboBox.Name = "colorSpacePickerComboBox";
            this.colorSpacePickerComboBox.Size = new System.Drawing.Size(281, 21);
            this.colorSpacePickerComboBox.TabIndex = 0;
            this.colorSpacePickerComboBox.SelectedIndexChanged += new System.EventHandler(this.colorSpacePickerComboBox_SelectedIndexChanged);
            // 
            // channelsGroupBox
            // 
            this.channelsGroupBox.Location = new System.Drawing.Point(3, 64);
            this.channelsGroupBox.Name = "channelsGroupBox";
            this.channelsGroupBox.Size = new System.Drawing.Size(294, 48);
            this.channelsGroupBox.TabIndex = 1;
            this.channelsGroupBox.TabStop = false;
            this.channelsGroupBox.Text = "Channels";
            // 
            // filterGroupBox
            // 
            this.filterGroupBox.Location = new System.Drawing.Point(3, 118);
            this.filterGroupBox.Name = "filterGroupBox";
            this.filterGroupBox.Size = new System.Drawing.Size(294, 133);
            this.filterGroupBox.TabIndex = 2;
            this.filterGroupBox.TabStop = false;
            this.filterGroupBox.Text = "Filters";
            // 
            // freqGroupBox
            // 
            this.freqGroupBox.Controls.Add(this.dctQuantizationQualityNumericUpDown);
            this.freqGroupBox.Controls.Add(this.dctUseQuantizationCheckBox);
            this.freqGroupBox.Controls.Add(this.dctZeroCoefNumericUpDown);
            this.freqGroupBox.Controls.Add(this.dctBlockSizeNumericUpDown);
            this.freqGroupBox.Controls.Add(this.dwtQuantizationCheckBox);
            this.freqGroupBox.Controls.Add(this.dwtQuantizationQuality);
            this.freqGroupBox.Controls.Add(this.dwtDepthNumericUpDown);
            this.freqGroupBox.Controls.Add(this.dwtSizeNumericUpDown);
            this.freqGroupBox.Controls.Add(this.freqRadioButtonDWT);
            this.freqGroupBox.Controls.Add(this.FreqRadioButtonDCT);
            this.freqGroupBox.Controls.Add(this.freqRadioButtonNone);
            this.freqGroupBox.Location = new System.Drawing.Point(3, 257);
            this.freqGroupBox.Name = "freqGroupBox";
            this.freqGroupBox.Size = new System.Drawing.Size(294, 108);
            this.freqGroupBox.TabIndex = 3;
            this.freqGroupBox.TabStop = false;
            this.freqGroupBox.Text = "Frequency filters";
            // 
            // dctQuantizationQualityNumericUpDown
            // 
            this.dctQuantizationQualityNumericUpDown.Location = new System.Drawing.Point(196, 42);
            this.dctQuantizationQualityNumericUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.dctQuantizationQualityNumericUpDown.Name = "dctQuantizationQualityNumericUpDown";
            this.dctQuantizationQualityNumericUpDown.Size = new System.Drawing.Size(42, 20);
            this.dctQuantizationQualityNumericUpDown.TabIndex = 10;
            this.controlsToolTip.SetToolTip(this.dctQuantizationQualityNumericUpDown, "DCT QuantizationQuality");
            this.dctQuantizationQualityNumericUpDown.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.dctQuantizationQualityNumericUpDown.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dctUseQuantizationCheckBox
            // 
            this.dctUseQuantizationCheckBox.AutoSize = true;
            this.dctUseQuantizationCheckBox.Location = new System.Drawing.Point(175, 44);
            this.dctUseQuantizationCheckBox.Name = "dctUseQuantizationCheckBox";
            this.dctUseQuantizationCheckBox.Size = new System.Drawing.Size(15, 14);
            this.dctUseQuantizationCheckBox.TabIndex = 9;
            this.controlsToolTip.SetToolTip(this.dctUseQuantizationCheckBox, "Use DCT quantization");
            this.dctUseQuantizationCheckBox.UseVisualStyleBackColor = true;
            this.dctUseQuantizationCheckBox.CheckedChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dctZeroCoefNumericUpDown
            // 
            this.dctZeroCoefNumericUpDown.Location = new System.Drawing.Point(117, 44);
            this.dctZeroCoefNumericUpDown.Maximum = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.dctZeroCoefNumericUpDown.Name = "dctZeroCoefNumericUpDown";
            this.dctZeroCoefNumericUpDown.Size = new System.Drawing.Size(51, 20);
            this.dctZeroCoefNumericUpDown.TabIndex = 8;
            this.controlsToolTip.SetToolTip(this.dctZeroCoefNumericUpDown, "DCT Zero coefficients below diagonal");
            this.dctZeroCoefNumericUpDown.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dctBlockSizeNumericUpDown
            // 
            this.dctBlockSizeNumericUpDown.Increment = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.dctBlockSizeNumericUpDown.Location = new System.Drawing.Point(65, 44);
            this.dctBlockSizeNumericUpDown.Maximum = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.dctBlockSizeNumericUpDown.Minimum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.dctBlockSizeNumericUpDown.Name = "dctBlockSizeNumericUpDown";
            this.dctBlockSizeNumericUpDown.Size = new System.Drawing.Size(45, 20);
            this.dctBlockSizeNumericUpDown.TabIndex = 7;
            this.controlsToolTip.SetToolTip(this.dctBlockSizeNumericUpDown, "DCT block size");
            this.dctBlockSizeNumericUpDown.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.dctBlockSizeNumericUpDown.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dwtQuantizationCheckBox
            // 
            this.dwtQuantizationCheckBox.AutoSize = true;
            this.dwtQuantizationCheckBox.Location = new System.Drawing.Point(175, 68);
            this.dwtQuantizationCheckBox.Name = "dwtQuantizationCheckBox";
            this.dwtQuantizationCheckBox.Size = new System.Drawing.Size(15, 14);
            this.dwtQuantizationCheckBox.TabIndex = 6;
            this.controlsToolTip.SetToolTip(this.dwtQuantizationCheckBox, "Use DWT quantization");
            this.dwtQuantizationCheckBox.UseVisualStyleBackColor = true;
            this.dwtQuantizationCheckBox.CheckedChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dwtQuantizationQuality
            // 
            this.dwtQuantizationQuality.Location = new System.Drawing.Point(196, 68);
            this.dwtQuantizationQuality.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.dwtQuantizationQuality.Name = "dwtQuantizationQuality";
            this.dwtQuantizationQuality.Size = new System.Drawing.Size(42, 20);
            this.dwtQuantizationQuality.TabIndex = 5;
            this.controlsToolTip.SetToolTip(this.dwtQuantizationQuality, "DWT quantization quality");
            this.dwtQuantizationQuality.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.dwtQuantizationQuality.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dwtDepthNumericUpDown
            // 
            this.dwtDepthNumericUpDown.Location = new System.Drawing.Point(117, 68);
            this.dwtDepthNumericUpDown.Name = "dwtDepthNumericUpDown";
            this.dwtDepthNumericUpDown.Size = new System.Drawing.Size(51, 20);
            this.dwtDepthNumericUpDown.TabIndex = 4;
            this.controlsToolTip.SetToolTip(this.dwtDepthNumericUpDown, "DWT depth");
            this.dwtDepthNumericUpDown.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.dwtDepthNumericUpDown.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // dwtSizeNumericUpDown
            // 
            this.dwtSizeNumericUpDown.Location = new System.Drawing.Point(65, 68);
            this.dwtSizeNumericUpDown.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.dwtSizeNumericUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.dwtSizeNumericUpDown.Name = "dwtSizeNumericUpDown";
            this.dwtSizeNumericUpDown.Size = new System.Drawing.Size(45, 20);
            this.dwtSizeNumericUpDown.TabIndex = 3;
            this.controlsToolTip.SetToolTip(this.dwtSizeNumericUpDown, "DWT block size");
            this.dwtSizeNumericUpDown.Value = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.dwtSizeNumericUpDown.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // freqRadioButtonDWT
            // 
            this.freqRadioButtonDWT.AutoSize = true;
            this.freqRadioButtonDWT.Location = new System.Drawing.Point(7, 68);
            this.freqRadioButtonDWT.Name = "freqRadioButtonDWT";
            this.freqRadioButtonDWT.Size = new System.Drawing.Size(51, 17);
            this.freqRadioButtonDWT.TabIndex = 2;
            this.freqRadioButtonDWT.Text = "DWT";
            this.freqRadioButtonDWT.UseVisualStyleBackColor = true;
            this.freqRadioButtonDWT.CheckedChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // FreqRadioButtonDCT
            // 
            this.FreqRadioButtonDCT.AutoSize = true;
            this.FreqRadioButtonDCT.Location = new System.Drawing.Point(7, 44);
            this.FreqRadioButtonDCT.Name = "FreqRadioButtonDCT";
            this.FreqRadioButtonDCT.Size = new System.Drawing.Size(47, 17);
            this.FreqRadioButtonDCT.TabIndex = 1;
            this.FreqRadioButtonDCT.Text = "DCT";
            this.FreqRadioButtonDCT.UseVisualStyleBackColor = true;
            this.FreqRadioButtonDCT.CheckedChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // freqRadioButtonNone
            // 
            this.freqRadioButtonNone.AutoSize = true;
            this.freqRadioButtonNone.Checked = true;
            this.freqRadioButtonNone.Location = new System.Drawing.Point(7, 20);
            this.freqRadioButtonNone.Name = "freqRadioButtonNone";
            this.freqRadioButtonNone.Size = new System.Drawing.Size(51, 17);
            this.freqRadioButtonNone.TabIndex = 0;
            this.freqRadioButtonNone.TabStop = true;
            this.freqRadioButtonNone.Text = "None";
            this.freqRadioButtonNone.UseVisualStyleBackColor = true;
            this.freqRadioButtonNone.CheckedChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // originalImageBox
            // 
            this.originalImageBox.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.originalImageBox.Location = new System.Drawing.Point(338, 27);
            this.originalImageBox.Name = "originalImageBox";
            this.originalImageBox.Size = new System.Drawing.Size(1012, 260);
            this.originalImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.originalImageBox.TabIndex = 2;
            this.originalImageBox.TabStop = false;
            this.originalImageBox.Click += new System.EventHandler(this.originalImageBox_Click);
            // 
            // darkPixelsPercentLabel
            // 
            this.darkPixelsPercentLabel.AutoSize = true;
            this.darkPixelsPercentLabel.Location = new System.Drawing.Point(338, 326);
            this.darkPixelsPercentLabel.Name = "darkPixelsPercentLabel";
            this.darkPixelsPercentLabel.Size = new System.Drawing.Size(98, 13);
            this.darkPixelsPercentLabel.TabIndex = 3;
            this.darkPixelsPercentLabel.Text = "Dark pixels percent";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1362, 741);
            this.Controls.Add(this.darkPixelsPercentLabel);
            this.Controls.Add(this.originalImageBox);
            this.Controls.Add(this.flowLayoutPanel1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Form1";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.colorSpaceGroupBox.ResumeLayout(false);
            this.freqGroupBox.ResumeLayout(false);
            this.freqGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dctQuantizationQualityNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dctZeroCoefNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dctBlockSizeNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dwtQuantizationQuality)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dwtDepthNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dwtSizeNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.originalImageBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openImageToolStripMenuItem;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.GroupBox colorSpaceGroupBox;
        private System.Windows.Forms.ComboBox colorSpacePickerComboBox;
        private System.Windows.Forms.GroupBox channelsGroupBox;
        private System.Windows.Forms.PictureBox originalImageBox;
        private System.Windows.Forms.GroupBox filterGroupBox;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fullImageToolStripMenuItem;
        private System.Windows.Forms.GroupBox freqGroupBox;
        private System.Windows.Forms.NumericUpDown dwtSizeNumericUpDown;
        private System.Windows.Forms.RadioButton freqRadioButtonDWT;
        private System.Windows.Forms.RadioButton FreqRadioButtonDCT;
        private System.Windows.Forms.RadioButton freqRadioButtonNone;
        private System.Windows.Forms.ToolTip controlsToolTip;
        private System.Windows.Forms.CheckBox dwtQuantizationCheckBox;
        private System.Windows.Forms.NumericUpDown dwtQuantizationQuality;
        private System.Windows.Forms.NumericUpDown dwtDepthNumericUpDown;
        private System.Windows.Forms.NumericUpDown dctBlockSizeNumericUpDown;
        private System.Windows.Forms.NumericUpDown dctQuantizationQualityNumericUpDown;
        private System.Windows.Forms.CheckBox dctUseQuantizationCheckBox;
        private System.Windows.Forms.NumericUpDown dctZeroCoefNumericUpDown;
        private System.Windows.Forms.Label darkPixelsPercentLabel;

    }
}

