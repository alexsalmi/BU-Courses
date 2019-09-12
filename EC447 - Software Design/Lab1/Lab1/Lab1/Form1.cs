using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            // Displaying my full name at coordinate 100, 20
            g.DrawString("Alexander Salmi", Font, Brushes.Black, 100, 20);

            // String variable for displaying the powers of 2
            string displayText;
            // y keeps track of the current y coordinate, and n keeps track of the current power of 2
            int y = 100 - Font.Height, n = 1;

            // Display the powers of 2 in a for loop
            for(int i=1; i<=16; i++)
            {
                // Update n to the next power of 2, and update the y coordinate
                n = n * 2;                                                  
                y = y + Font.Height;
                // Use String.Format to create the display string
                displayText = String.Format("{0} {1}", i, n);
                // Displaying the current power of 2
                g.DrawString(displayText, Font, Brushes.Black, 100, y);
            }
        }
    }
}
