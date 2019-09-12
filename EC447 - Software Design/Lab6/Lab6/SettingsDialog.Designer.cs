namespace Lab6
{
    partial class SettingsDialog
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
            this.penColorLabel = new System.Windows.Forms.Label();
            this.penColorBox = new System.Windows.Forms.ListBox();
            this.fillColorLabel = new System.Windows.Forms.Label();
            this.fillColorBox = new System.Windows.Forms.ListBox();
            this.penWidthLabel = new System.Windows.Forms.Label();
            this.penWidthBox = new System.Windows.Forms.ListBox();
            this.fillButton = new System.Windows.Forms.CheckBox();
            this.outlineButton = new System.Windows.Forms.CheckBox();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // penColorLabel
            // 
            this.penColorLabel.AutoSize = true;
            this.penColorLabel.Location = new System.Drawing.Point(47, 49);
            this.penColorLabel.Name = "penColorLabel";
            this.penColorLabel.Size = new System.Drawing.Size(70, 17);
            this.penColorLabel.TabIndex = 0;
            this.penColorLabel.Text = "Pen Color";
            // 
            // penColorBox
            // 
            this.penColorBox.FormattingEnabled = true;
            this.penColorBox.ItemHeight = 16;
            this.penColorBox.Items.AddRange(new object[] {
            "Black",
            "Red",
            "Blue",
            "Green"});
            this.penColorBox.Location = new System.Drawing.Point(50, 83);
            this.penColorBox.Name = "penColorBox";
            this.penColorBox.Size = new System.Drawing.Size(84, 84);
            this.penColorBox.TabIndex = 1;
            // 
            // fillColorLabel
            // 
            this.fillColorLabel.AutoSize = true;
            this.fillColorLabel.Location = new System.Drawing.Point(172, 49);
            this.fillColorLabel.Name = "fillColorLabel";
            this.fillColorLabel.Size = new System.Drawing.Size(62, 17);
            this.fillColorLabel.TabIndex = 2;
            this.fillColorLabel.Text = "Fill Color";
            // 
            // fillColorBox
            // 
            this.fillColorBox.FormattingEnabled = true;
            this.fillColorBox.ItemHeight = 16;
            this.fillColorBox.Items.AddRange(new object[] {
            "White",
            "Black",
            "Red",
            "Blue",
            "Green"});
            this.fillColorBox.Location = new System.Drawing.Point(175, 83);
            this.fillColorBox.Name = "fillColorBox";
            this.fillColorBox.Size = new System.Drawing.Size(84, 84);
            this.fillColorBox.TabIndex = 3;
            // 
            // penWidthLabel
            // 
            this.penWidthLabel.AutoSize = true;
            this.penWidthLabel.Location = new System.Drawing.Point(313, 49);
            this.penWidthLabel.Name = "penWidthLabel";
            this.penWidthLabel.Size = new System.Drawing.Size(73, 17);
            this.penWidthLabel.TabIndex = 4;
            this.penWidthLabel.Text = "Pen Width";
            // 
            // penWidthBox
            // 
            this.penWidthBox.FormattingEnabled = true;
            this.penWidthBox.ItemHeight = 16;
            this.penWidthBox.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"});
            this.penWidthBox.Location = new System.Drawing.Point(316, 83);
            this.penWidthBox.Name = "penWidthBox";
            this.penWidthBox.Size = new System.Drawing.Size(157, 164);
            this.penWidthBox.TabIndex = 5;
            // 
            // fillButton
            // 
            this.fillButton.AutoSize = true;
            this.fillButton.Location = new System.Drawing.Point(140, 226);
            this.fillButton.Name = "fillButton";
            this.fillButton.Size = new System.Drawing.Size(47, 21);
            this.fillButton.TabIndex = 6;
            this.fillButton.Text = "Fill";
            this.fillButton.UseVisualStyleBackColor = true;
            // 
            // outlineButton
            // 
            this.outlineButton.AutoSize = true;
            this.outlineButton.Location = new System.Drawing.Point(140, 253);
            this.outlineButton.Name = "outlineButton";
            this.outlineButton.Size = new System.Drawing.Size(75, 21);
            this.outlineButton.TabIndex = 7;
            this.outlineButton.Text = "Outline";
            this.outlineButton.UseVisualStyleBackColor = true;
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(244, 291);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(100, 28);
            this.okButton.TabIndex = 0;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(395, 291);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(100, 28);
            this.cancelButton.TabIndex = 1;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // SettingsDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(532, 378);
            this.ControlBox = false;
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.outlineButton);
            this.Controls.Add(this.fillButton);
            this.Controls.Add(this.penWidthBox);
            this.Controls.Add(this.penWidthLabel);
            this.Controls.Add(this.fillColorBox);
            this.Controls.Add(this.fillColorLabel);
            this.Controls.Add(this.penColorBox);
            this.Controls.Add(this.penColorLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SettingsDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Settings";
            this.Load += new System.EventHandler(this.SettingsDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label penColorLabel;
        private System.Windows.Forms.ListBox penColorBox;
        private System.Windows.Forms.Label fillColorLabel;
        private System.Windows.Forms.ListBox fillColorBox;
        private System.Windows.Forms.Label penWidthLabel;
        private System.Windows.Forms.ListBox penWidthBox;
        private System.Windows.Forms.CheckBox fillButton;
        private System.Windows.Forms.CheckBox outlineButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
    }
}