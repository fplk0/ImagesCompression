namespace Lab1
{
    partial class ColorChannelControl
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
            this.selectedCheckBox = new System.Windows.Forms.CheckBox();
            this.brightnessAddTrackBar = new System.Windows.Forms.TrackBar();
            this.brightnessMultiplyTrackBar = new System.Windows.Forms.TrackBar();
            this.gammaPicker = new System.Windows.Forms.NumericUpDown();
            this.gammaLabel = new System.Windows.Forms.Label();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.channelNameLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.brightnessAddTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.brightnessMultiplyTrackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gammaPicker)).BeginInit();
            this.SuspendLayout();
            // 
            // selectedCheckBox
            // 
            this.selectedCheckBox.AutoSize = true;
            this.selectedCheckBox.Checked = true;
            this.selectedCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.selectedCheckBox.Location = new System.Drawing.Point(3, 18);
            this.selectedCheckBox.Name = "selectedCheckBox";
            this.selectedCheckBox.Size = new System.Drawing.Size(15, 14);
            this.selectedCheckBox.TabIndex = 1;
            this.selectedCheckBox.UseVisualStyleBackColor = true;
            this.selectedCheckBox.CheckedChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // brightnessAddTrackBar
            // 
            this.brightnessAddTrackBar.LargeChange = 20;
            this.brightnessAddTrackBar.Location = new System.Drawing.Point(80, 3);
            this.brightnessAddTrackBar.Maximum = 100;
            this.brightnessAddTrackBar.Minimum = -100;
            this.brightnessAddTrackBar.Name = "brightnessAddTrackBar";
            this.brightnessAddTrackBar.Size = new System.Drawing.Size(70, 45);
            this.brightnessAddTrackBar.TabIndex = 2;
            this.brightnessAddTrackBar.TickFrequency = 20;
            this.brightnessAddTrackBar.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // brightnessMultiplyTrackBar
            // 
            this.brightnessMultiplyTrackBar.LargeChange = 20;
            this.brightnessMultiplyTrackBar.Location = new System.Drawing.Point(156, 3);
            this.brightnessMultiplyTrackBar.Maximum = 100;
            this.brightnessMultiplyTrackBar.Minimum = -100;
            this.brightnessMultiplyTrackBar.Name = "brightnessMultiplyTrackBar";
            this.brightnessMultiplyTrackBar.Size = new System.Drawing.Size(70, 45);
            this.brightnessMultiplyTrackBar.TabIndex = 3;
            this.brightnessMultiplyTrackBar.TickFrequency = 20;
            this.brightnessMultiplyTrackBar.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // gammaPicker
            // 
            this.gammaPicker.DecimalPlaces = 2;
            this.gammaPicker.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.gammaPicker.Location = new System.Drawing.Point(232, 28);
            this.gammaPicker.Name = "gammaPicker";
            this.gammaPicker.Size = new System.Drawing.Size(55, 20);
            this.gammaPicker.TabIndex = 4;
            this.gammaPicker.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.gammaPicker.ValueChanged += new System.EventHandler(this.controlValueChanged);
            // 
            // gammaLabel
            // 
            this.gammaLabel.AutoSize = true;
            this.gammaLabel.Location = new System.Drawing.Point(229, 3);
            this.gammaLabel.Name = "gammaLabel";
            this.gammaLabel.Size = new System.Drawing.Size(43, 13);
            this.gammaLabel.TabIndex = 5;
            this.gammaLabel.Text = "Gamma";
            // 
            // channelNameLabel
            // 
            this.channelNameLabel.AutoSize = true;
            this.channelNameLabel.Location = new System.Drawing.Point(25, 18);
            this.channelNameLabel.Name = "channelNameLabel";
            this.channelNameLabel.Size = new System.Drawing.Size(48, 13);
            this.channelNameLabel.TabIndex = 6;
            this.channelNameLabel.Text = "ChName";
            // 
            // ColorChannelControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.channelNameLabel);
            this.Controls.Add(this.gammaLabel);
            this.Controls.Add(this.gammaPicker);
            this.Controls.Add(this.brightnessMultiplyTrackBar);
            this.Controls.Add(this.brightnessAddTrackBar);
            this.Controls.Add(this.selectedCheckBox);
            this.Name = "ColorChannelControl";
            this.Size = new System.Drawing.Size(290, 53);
            ((System.ComponentModel.ISupportInitialize)(this.brightnessAddTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.brightnessMultiplyTrackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gammaPicker)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox selectedCheckBox;
        private System.Windows.Forms.TrackBar brightnessAddTrackBar;
        private System.Windows.Forms.TrackBar brightnessMultiplyTrackBar;
        private System.Windows.Forms.NumericUpDown gammaPicker;
        private System.Windows.Forms.Label gammaLabel;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.Label channelNameLabel;
    }
}
