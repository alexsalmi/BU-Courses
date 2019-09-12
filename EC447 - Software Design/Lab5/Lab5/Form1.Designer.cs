namespace Lab5
{
    partial class Form1
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
            this.startIntBox = new System.Windows.Forms.TextBox();
            this.countBox = new System.Windows.Forms.TextBox();
            this.startIntLabel = new System.Windows.Forms.Label();
            this.countLabel = new System.Windows.Forms.Label();
            this.generateButton = new System.Windows.Forms.Button();
            this.errorLabel = new System.Windows.Forms.Label();
            this.palindromeBox = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // startIntBox
            // 
            this.startIntBox.Location = new System.Drawing.Point(460, 130);
            this.startIntBox.Name = "startIntBox";
            this.startIntBox.Size = new System.Drawing.Size(130, 22);
            this.startIntBox.TabIndex = 0;
            // 
            // countBox
            // 
            this.countBox.Location = new System.Drawing.Point(795, 130);
            this.countBox.Name = "countBox";
            this.countBox.Size = new System.Drawing.Size(75, 22);
            this.countBox.TabIndex = 1;
            // 
            // startIntLabel
            // 
            this.startIntLabel.AutoSize = true;
            this.startIntLabel.Location = new System.Drawing.Point(180, 133);
            this.startIntLabel.Name = "startIntLabel";
            this.startIntLabel.Size = new System.Drawing.Size(276, 17);
            this.startIntLabel.TabIndex = 2;
            this.startIntLabel.Text = "Enter a starting integer (0-1,000,000,000):";
            // 
            // countLabel
            // 
            this.countLabel.AutoSize = true;
            this.countLabel.Location = new System.Drawing.Point(650, 133);
            this.countLabel.Name = "countLabel";
            this.countLabel.Size = new System.Drawing.Size(138, 17);
            this.countLabel.TabIndex = 3;
            this.countLabel.Text = "Enter Count (1-100):";
            // 
            // generateButton
            // 
            this.generateButton.Location = new System.Drawing.Point(475, 190);
            this.generateButton.Name = "generateButton";
            this.generateButton.Size = new System.Drawing.Size(100, 30);
            this.generateButton.TabIndex = 4;
            this.generateButton.Text = "Generate";
            this.generateButton.UseVisualStyleBackColor = true;
            this.generateButton.Click += new System.EventHandler(this.generateButton_Click);
            // 
            // errorLabel
            // 
            this.errorLabel.AutoSize = true;
            this.errorLabel.Location = new System.Drawing.Point(385, 435);
            this.errorLabel.Name = "errorLabel";
            this.errorLabel.Size = new System.Drawing.Size(284, 17);
            this.errorLabel.TabIndex = 6;
            this.errorLabel.Text = "Please enter a positive integer within range.";
            this.errorLabel.Visible = false;
            // 
            // palindromeBox
            // 
            this.palindromeBox.FormattingEnabled = true;
            this.palindromeBox.ItemHeight = 16;
            this.palindromeBox.Location = new System.Drawing.Point(445, 240);
            this.palindromeBox.Name = "palindromeBox";
            this.palindromeBox.Size = new System.Drawing.Size(160, 164);
            this.palindromeBox.TabIndex = 7;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1054, 491);
            this.Controls.Add(this.palindromeBox);
            this.Controls.Add(this.errorLabel);
            this.Controls.Add(this.generateButton);
            this.Controls.Add(this.countLabel);
            this.Controls.Add(this.startIntLabel);
            this.Controls.Add(this.countBox);
            this.Controls.Add(this.startIntBox);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox startIntBox;
        private System.Windows.Forms.TextBox countBox;
        private System.Windows.Forms.Label startIntLabel;
        private System.Windows.Forms.Label countLabel;
        private System.Windows.Forms.Button generateButton;
        private System.Windows.Forms.Label errorLabel;
        private System.Windows.Forms.ListBox palindromeBox;
    }
}

