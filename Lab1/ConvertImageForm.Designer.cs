namespace Lab1
{
    partial class ConvertImageForm
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
            this.qualityField = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.yHor = new System.Windows.Forms.ComboBox();
            this.yVert = new System.Windows.Forms.ComboBox();
            this.cbHor = new System.Windows.Forms.ComboBox();
            this.cbVert = new System.Windows.Forms.ComboBox();
            this.crHor = new System.Windows.Forms.ComboBox();
            this.crVert = new System.Windows.Forms.ComboBox();
            this.destinationFilePath = new System.Windows.Forms.TextBox();
            this.selectFileButton = new System.Windows.Forms.Button();
            this.saveButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.qualityField)).BeginInit();
            this.SuspendLayout();
            // 
            // qualityField
            // 
            this.qualityField.Location = new System.Drawing.Point(13, 13);
            this.qualityField.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.qualityField.Name = "qualityField";
            this.qualityField.Size = new System.Drawing.Size(51, 20);
            this.qualityField.TabIndex = 0;
            this.qualityField.Value = new decimal(new int[] {
            90,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 58);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Y Horizontal";
            // 
            // yHor
            // 
            this.yHor.DisplayMember = "2";
            this.yHor.FormattingEnabled = true;
            this.yHor.Items.AddRange(new object[] {
            "1",
            "2",
            "4"});
            this.yHor.Location = new System.Drawing.Point(83, 55);
            this.yHor.Name = "yHor";
            this.yHor.Size = new System.Drawing.Size(51, 21);
            this.yHor.TabIndex = 2;
            this.yHor.Text = "2";
            // 
            // yVert
            // 
            this.yVert.FormattingEnabled = true;
            this.yVert.Items.AddRange(new object[] {
            "1",
            "2",
            "4"});
            this.yVert.Location = new System.Drawing.Point(264, 58);
            this.yVert.Name = "yVert";
            this.yVert.Size = new System.Drawing.Size(51, 21);
            this.yVert.TabIndex = 3;
            this.yVert.Text = "2";
            // 
            // cbHor
            // 
            this.cbHor.FormattingEnabled = true;
            this.cbHor.Items.AddRange(new object[] {
            "1",
            "2",
            "4"});
            this.cbHor.Location = new System.Drawing.Point(83, 91);
            this.cbHor.Name = "cbHor";
            this.cbHor.Size = new System.Drawing.Size(51, 21);
            this.cbHor.TabIndex = 4;
            this.cbHor.Text = "1";
            // 
            // cbVert
            // 
            this.cbVert.FormattingEnabled = true;
            this.cbVert.Items.AddRange(new object[] {
            "1",
            "2",
            "4"});
            this.cbVert.Location = new System.Drawing.Point(264, 91);
            this.cbVert.Name = "cbVert";
            this.cbVert.Size = new System.Drawing.Size(51, 21);
            this.cbVert.TabIndex = 5;
            this.cbVert.Text = "1";
            // 
            // crHor
            // 
            this.crHor.FormattingEnabled = true;
            this.crHor.Items.AddRange(new object[] {
            "1",
            "2",
            "4"});
            this.crHor.Location = new System.Drawing.Point(83, 131);
            this.crHor.Name = "crHor";
            this.crHor.Size = new System.Drawing.Size(51, 21);
            this.crHor.TabIndex = 6;
            this.crHor.Text = "1";
            // 
            // crVert
            // 
            this.crVert.FormattingEnabled = true;
            this.crVert.Items.AddRange(new object[] {
            "1",
            "2",
            "4"});
            this.crVert.Location = new System.Drawing.Point(264, 131);
            this.crVert.Name = "crVert";
            this.crVert.Size = new System.Drawing.Size(51, 21);
            this.crVert.TabIndex = 7;
            this.crVert.Text = "1";
            // 
            // destinationFilePath
            // 
            this.destinationFilePath.Location = new System.Drawing.Point(16, 180);
            this.destinationFilePath.Name = "destinationFilePath";
            this.destinationFilePath.Size = new System.Drawing.Size(299, 20);
            this.destinationFilePath.TabIndex = 8;
            // 
            // selectFileButton
            // 
            this.selectFileButton.Location = new System.Drawing.Point(359, 180);
            this.selectFileButton.Name = "selectFileButton";
            this.selectFileButton.Size = new System.Drawing.Size(75, 23);
            this.selectFileButton.TabIndex = 9;
            this.selectFileButton.Text = "Select file";
            this.selectFileButton.UseVisualStyleBackColor = true;
            this.selectFileButton.Click += new System.EventHandler(this.selectFileButton_Click);
            // 
            // saveButton
            // 
            this.saveButton.Location = new System.Drawing.Point(13, 209);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 23);
            this.saveButton.TabIndex = 10;
            this.saveButton.Text = "Save";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // ConvertImageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 244);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.selectFileButton);
            this.Controls.Add(this.destinationFilePath);
            this.Controls.Add(this.crVert);
            this.Controls.Add(this.crHor);
            this.Controls.Add(this.cbVert);
            this.Controls.Add(this.cbHor);
            this.Controls.Add(this.yVert);
            this.Controls.Add(this.yHor);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.qualityField);
            this.Name = "ConvertImageForm";
            this.Text = "ConvertImageForm";
            ((System.ComponentModel.ISupportInitialize)(this.qualityField)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown qualityField;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox yHor;
        private System.Windows.Forms.ComboBox yVert;
        private System.Windows.Forms.ComboBox cbHor;
        private System.Windows.Forms.ComboBox cbVert;
        private System.Windows.Forms.ComboBox crHor;
        private System.Windows.Forms.ComboBox crVert;
        private System.Windows.Forms.TextBox destinationFilePath;
        private System.Windows.Forms.Button selectFileButton;
        private System.Windows.Forms.Button saveButton;
    }
}