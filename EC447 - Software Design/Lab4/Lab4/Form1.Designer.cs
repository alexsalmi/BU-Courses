namespace Lab4
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
            this.FindSolution = new System.Windows.Forms.Button();
            this.StartRow = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.StartRow)).BeginInit();
            this.SuspendLayout();
            // 
            // FindSolution
            // 
            this.FindSolution.Location = new System.Drawing.Point(475, 25);
            this.FindSolution.Name = "FindSolution";
            this.FindSolution.Size = new System.Drawing.Size(125, 25);
            this.FindSolution.TabIndex = 0;
            this.FindSolution.Text = "Find Solution";
            this.FindSolution.UseVisualStyleBackColor = true;
            this.FindSolution.Click += new System.EventHandler(this.FindSolution_Click);
            // 
            // StartRow
            // 
            this.StartRow.Location = new System.Drawing.Point(350, 25);
            this.StartRow.Maximum = new decimal(new int[] {
            7,
            0,
            0,
            0});
            this.StartRow.Name = "StartRow";
            this.StartRow.Size = new System.Drawing.Size(50, 22);
            this.StartRow.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(200, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(128, 17);
            this.label1.TabIndex = 2;
            this.label1.Text = "Select starting row:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(837, 748);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.StartRow);
            this.Controls.Add(this.FindSolution);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.StartRow)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button FindSolution;
        private System.Windows.Forms.NumericUpDown StartRow;
        private System.Windows.Forms.Label label1;
    }
}

