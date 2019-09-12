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

namespace Lab2
{
    public partial class Form1 : Form
    {
        ArrayList clickCoordinates = new ArrayList();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_MouseClick_1(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Point point = new Point(e.X, e.Y);
                this.clickCoordinates.Add(point);
                this.Invalidate();
            }
            else if(e.Button == MouseButtons.Right)
            {
                this.clickCoordinates.Clear();
                this.Invalidate();
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            const int WIDTH = 5;
            const int HEIGHT = 5;
            if (this.clickCoordinates.Count != 0)
            {
                Point prevPoint = (Point)this.clickCoordinates[0];
                foreach (Point point in this.clickCoordinates)
                {
                    e.Graphics.FillEllipse(Brushes.Black, point.X - WIDTH / 2, point.Y - WIDTH / 2, WIDTH, HEIGHT);
                    e.Graphics.DrawLine(Pens.Black, point, prevPoint);
                    prevPoint = point;
                }
            }
        }
    }
}
