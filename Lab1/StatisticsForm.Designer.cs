namespace Lab1
{
    partial class StatisticsForm
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
            this.statGridView = new System.Windows.Forms.DataGridView();
            this.pathTextBox = new System.Windows.Forms.TextBox();
            this.selectFolderButton = new System.Windows.Forms.Button();
            this.runButton = new System.Windows.Forms.Button();
            this.qualityImageName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.compressionTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.decompressionTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.symmetry = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.compressionRatio = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.psnr = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.statGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // statGridView
            // 
            this.statGridView.AllowUserToAddRows = false;
            this.statGridView.AllowUserToDeleteRows = false;
            this.statGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.statGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.qualityImageName,
            this.compressionTime,
            this.decompressionTime,
            this.symmetry,
            this.compressionRatio,
            this.psnr});
            this.statGridView.Location = new System.Drawing.Point(12, 47);
            this.statGridView.Name = "statGridView";
            this.statGridView.ReadOnly = true;
            this.statGridView.RowHeadersVisible = false;
            this.statGridView.Size = new System.Drawing.Size(808, 403);
            this.statGridView.TabIndex = 0;
            // 
            // pathTextBox
            // 
            this.pathTextBox.Location = new System.Drawing.Point(12, 12);
            this.pathTextBox.Name = "pathTextBox";
            this.pathTextBox.Size = new System.Drawing.Size(456, 20);
            this.pathTextBox.TabIndex = 1;
            this.pathTextBox.Text = "D:\\TestImages";
            // 
            // selectFolderButton
            // 
            this.selectFolderButton.Location = new System.Drawing.Point(474, 10);
            this.selectFolderButton.Name = "selectFolderButton";
            this.selectFolderButton.Size = new System.Drawing.Size(75, 23);
            this.selectFolderButton.TabIndex = 2;
            this.selectFolderButton.Text = "Select folder";
            this.selectFolderButton.UseVisualStyleBackColor = true;
            this.selectFolderButton.Click += new System.EventHandler(this.selectFolderButton_Click);
            // 
            // runButton
            // 
            this.runButton.Location = new System.Drawing.Point(555, 9);
            this.runButton.Name = "runButton";
            this.runButton.Size = new System.Drawing.Size(75, 23);
            this.runButton.TabIndex = 3;
            this.runButton.Text = "Run";
            this.runButton.UseVisualStyleBackColor = true;
            this.runButton.Click += new System.EventHandler(this.runButton_Click);
            // 
            // qualityImageName
            // 
            this.qualityImageName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.qualityImageName.HeaderText = "Quality/Image name";
            this.qualityImageName.Name = "qualityImageName";
            this.qualityImageName.ReadOnly = true;
            this.qualityImageName.Width = 116;
            // 
            // compressionTime
            // 
            this.compressionTime.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.compressionTime.HeaderText = "Compression time";
            this.compressionTime.Name = "compressionTime";
            this.compressionTime.ReadOnly = true;
            this.compressionTime.Width = 105;
            // 
            // decompressionTime
            // 
            this.decompressionTime.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.decompressionTime.HeaderText = "Decompression time";
            this.decompressionTime.Name = "decompressionTime";
            this.decompressionTime.ReadOnly = true;
            this.decompressionTime.Width = 116;
            // 
            // symmetry
            // 
            this.symmetry.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.symmetry.HeaderText = "Symmetry";
            this.symmetry.Name = "symmetry";
            this.symmetry.ReadOnly = true;
            this.symmetry.Width = 77;
            // 
            // compressionRatio
            // 
            this.compressionRatio.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.compressionRatio.HeaderText = "Compression ratio";
            this.compressionRatio.Name = "compressionRatio";
            this.compressionRatio.ReadOnly = true;
            this.compressionRatio.Width = 105;
            // 
            // psnr
            // 
            this.psnr.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.psnr.HeaderText = "PSNR";
            this.psnr.Name = "psnr";
            this.psnr.ReadOnly = true;
            this.psnr.Width = 62;
            // 
            // StatisticsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(832, 462);
            this.Controls.Add(this.runButton);
            this.Controls.Add(this.selectFolderButton);
            this.Controls.Add(this.pathTextBox);
            this.Controls.Add(this.statGridView);
            this.Name = "StatisticsForm";
            this.Text = "StatisticsForm";
            ((System.ComponentModel.ISupportInitialize)(this.statGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView statGridView;
        private System.Windows.Forms.TextBox pathTextBox;
        private System.Windows.Forms.Button selectFolderButton;
        private System.Windows.Forms.Button runButton;
        private System.Windows.Forms.DataGridViewTextBoxColumn qualityImageName;
        private System.Windows.Forms.DataGridViewTextBoxColumn compressionTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn decompressionTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn symmetry;
        private System.Windows.Forms.DataGridViewTextBoxColumn compressionRatio;
        private System.Windows.Forms.DataGridViewTextBoxColumn psnr;
    }
}