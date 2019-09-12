using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab7
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        // For when the user clicks the browse button
        private void browseButton_Click(object sender, EventArgs e)
        {
            // Open dialog to chose a file
            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
                fileNameBox.Text = openFileDialog.FileName;
        }

        // For when the user clicks the encrypt button
        private void encryptButton_Click(object sender, EventArgs e)
        {
            // If no key has been entered, show an error and return
            if (keyBox.Text == "")
            {
                MessageBox.Show("Please enter a key.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            
            // Run the encryption on the files
            encryptionAlgorithm(fileNameBox.Text, fileNameBox.Text + ".enc", keyBox.Text);
        }

        private void decryptButton_Click(object sender, EventArgs e)
        {
            // If no key has been entered, show an error and return
            if (keyBox.Text == "")
            {
                MessageBox.Show("Please enter a key.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            // If the input file is not a .enc file, show an error and return
            else if (fileNameBox.Text.Length < 4 || fileNameBox.Text.Substring(fileNameBox.Text.Length - 4) != ".enc")
            {
                MessageBox.Show("Not a .enc file.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Run the encryption on the files
            encryptionAlgorithm(fileNameBox.Text, fileNameBox.Text.Substring(0, fileNameBox.Text.Length-4), keyBox.Text);
        }

        // Function for encrypting the files
        public void encryptionAlgorithm(string inFile, string outFile, string key)
        {
            // Initialize filestreams
            FileStream fin = null;
            FileStream fout = null;

            // If the output file already exists, ask if they want to overwrite it
            if (File.Exists(outFile))
                if (MessageBox.Show("Output file exists. Overwrite?", "File Exists", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
                    return;

            // Create the FileStreams
            try
            {
                fin = new FileStream(inFile, FileMode.Open);
                fout = new FileStream(outFile, FileMode.Create);
            }
            // If either file does not exist, show error dialog
            catch (Exception ee)
            {
                MessageBox.Show("Could not open source or destination file.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            int rbyte;
            int pos = 0;    //position in key string
            int length = key.Length; //length of key
            byte kbyte, ebyte; //encrypted byte

            while ((rbyte = fin.ReadByte()) != -1)
            {
                kbyte = (byte)key[pos];
                ebyte = (byte)(rbyte ^ kbyte);
                fout.WriteByte(ebyte);
                ++pos;
                if (pos == length)
                    pos = 0;
            }

            // Show a message box saying the encryption is complete
            MessageBox.Show("Operation completed successfully.", "", MessageBoxButtons.OK);

            // Close the files
            if (fin != null) fin.Close();
            if (fout != null) fout.Close();
        }
    }
}
