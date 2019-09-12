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

namespace Lab6
{
    public partial class Form1 : Form
    {
        // Initialize all necessary variables
        private string fillColor, penColor;
        private int penSize;
        private bool fill, outline, firstClick;
        private Point firstPoint;
        private ArrayList shapeList = new ArrayList();

        public Form1()
        {
            // Set all setting variables to their default values
            fillColor = "White";
            penColor = "Black";
            penSize = 1;
            fill = false;
            outline = true;
            firstClick = true;
            InitializeComponent();
        }

        // When the user clicks the settings button
        private void settingButton_Click(object sender, EventArgs e)
        {
            // Start a new setting dialog, and set all the options to their current values
            SettingsDialog settings = new SettingsDialog();
            settings.fillColor = fillColor;
            settings.penColor = penColor;
            settings.penSize = penSize;
            settings.fill = fill;
            settings.outline = outline;

            // If the user exits the dialog by pressing the OK button, update the current setting variables
            if(settings.ShowDialog() == DialogResult.OK)
            {
                fillColor = settings.fillColor;
                penColor = settings.penColor;
                penSize = settings.penSize;
                fill = settings.fill;
                outline = settings.outline;
            }
        }

        // When the user clicks on the drawing panel (drawing)
        private void drawPanel_MouseClick(object sender, MouseEventArgs e)
        {
            // If this is the first of the two clicks, store the coordinates of this click, and update the state variable
            if (firstClick)
            {
                firstPoint = new Point(e.X, e.Y);
                firstClick = false;
            }
            // If it is the second of two clicks
            else
            {
                // Save the coordinates of this second click
                Point newPoint = new Point(e.X, e.Y);

                // If the user is drawing a line, simply draw the line using the two point coordinates
                if (lineButton.Checked)
                    shapeList.Add(new Line(penColor, penSize, firstPoint, newPoint));
                // If the user is drawing a rectangle or ellipse
                else
                {
                    // Calculate the coordinates of the top left corner of the shape, and the size of the shape's bounding rectangle
                    int minX, minY, sizeX, sizeY;
                    if (firstPoint.X < newPoint.X)
                    {
                        minX = firstPoint.X;
                        sizeX = newPoint.X - minX;
                    }
                    else
                    {
                        minX = newPoint.X;
                        sizeX = firstPoint.X - minX;
                    }

                    if (firstPoint.Y < newPoint.Y)
                    {
                        minY = firstPoint.Y;
                        sizeY = newPoint.Y - minY;
                    }
                    else
                    {
                        minY = newPoint.Y;
                        sizeY = firstPoint.Y - minY;
                    }

                    Point loc = new Point(minX, minY);
                    Size size = new Size(sizeX, sizeY);

                    // If either the fill or outline settings are checked, draw the correct shape using the point and bounding rectangle just calculated 
                    if(fill || outline)
                    {
                        if (rectangleButton.Checked)
                            shapeList.Add(new Rect(penColor, fillColor, penSize, loc, size, fill, outline));
                        if (ellipseButton.Checked)
                            shapeList.Add(new Ellipse(penColor, fillColor, penSize, loc, size, fill, outline));
                    }
                    // If neither the fill or outline settings are checked, display an error message
                    else
                    {
                        MessageBox.Show("Fill and or outline must be checked.");
                    }
                }

                // Update the state variable
                firstClick = true;
            }

            // Update the drawing panel to display the new shapes
            this.drawPanel.Invalidate();
        }

        // When the user clicks the undo menu button
        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Remove the last drawn shape
            if (shapeList.Count != 0)
                shapeList.RemoveAt(shapeList.Count - 1);

            // Update the drawing panel to remove the last shape
            this.drawPanel.Invalidate();
        }

        // When the user clicks the clear menu button
        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Remove all the drawn shapes
            shapeList.Clear();

            // Update the drawing panel to remove all the shapes
            this.drawPanel.Invalidate();
        }

        // When the user clicks the exit menu item
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Exit the program
            this.Close();
        }

        // Painting the drawing panel
        private void drawPanel_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            // If we are currently waiting for the second click, we need to display a circle at the location of the first click
            if (!firstClick)
            {
                int width = 10, height = 10;
                g.FillEllipse(Brushes.Black, firstPoint.X - width / 2, firstPoint.Y - height / 2, width, height);
            }

            // Draw all of the shapes that are stored in the shapeList
            foreach(Shapes s in shapeList)
            {
                s.Draw(g);
            }
        }
    }
}
