using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab5
{
    public partial class Form1 : Form
    {
        private Font titleFont;

        public Form1()
        {
            InitializeComponent();
            this.Text = "Palindromes by Alexander Salmi";
            titleFont = new Font("Sans Serif", 24, FontStyle.Bold);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = e.Graphics;

            // Print the header
            g.DrawString("Find Numeric Palindromes", titleFont,  Brushes.Black, 190, 25);
        }

        // Function for when button is pressed
        private void generateButton_Click(object sender, EventArgs e)
        {
            int startInt, count;    // Variables to store the user input
            palindromeBox.Items.Clear();    // Clear the current palindromes being displayed

            // If TryParse returns true, the user input are integers
            if (int.TryParse(startIntBox.Text, out startInt) && int.TryParse(countBox.Text, out count))
            {
                // If the user inputted numbers are out of range, display the error message
                if (startInt < 0 || startInt > 1000000000 || count < 1 || count > 100)
                    errorLabel.Visible = true;
                // If the user inputted numbers are within range, hide the error message and find the palindromes using the generatePalindromes function
                else
                {
                    errorLabel.Visible = false;
                    generatePalindromes(startInt, count);
                }
            }
            // If TryParse returned false, the user inputs are not integers, so display the error message
            else
                errorLabel.Visible = true;

            Invalidate(); 
        }

        // Function to find the palindromes and add them to the listBox's items
        private void generatePalindromes(int number, int count)
        {
            string currentNum, reverse;     // Variables to store the current number we are checking in string form, and its reverse

            // Continue finding palindromes until we have the desired amount
            while(count > 0)
            {
                // Reset the reverse variable, and convert the current number we are checking to a string
                reverse = "";
                currentNum = number.ToString();

                // Get the reverse of the current number string
                for(int i=currentNum.Length - 1; i>=0; i--)
                    reverse += currentNum[i];

                // If the current number and its string are the same, we have a palindrome
                if (reverse == currentNum)
                {
                    palindromeBox.Items.Add(currentNum);    // Add the palindrome to the listBox's items
                    count--;    // Decrement the number of palindromes we still need to find
                }

                // Increment the current number we are checking
                number++;
            }
        }
    }
}
