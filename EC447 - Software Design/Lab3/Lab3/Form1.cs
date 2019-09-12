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

namespace Lab3
{
    public partial class Form1 : Form
    {
        ArrayList clickCoordinates = new ArrayList();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Lab3_Point point = new Lab3_Point(e.X, e.Y);
                this.clickCoordinates.Add(point);
                this.Invalidate();
            }
            else if (e.Button == MouseButtons.Right)
            {
                const int WIDTH = 20;
                const int HEIGHT = 20;
                foreach(Lab3_Point point in this.clickCoordinates)
                {
                    if (point.coordinates.X + WIDTH/2 > e.X && point.coordinates.X - WIDTH / 2 < e.X &&
                        point.coordinates.Y + HEIGHT/2 > e.Y && point.coordinates.Y - HEIGHT / 2 < e.Y)
                    {
                        if (point.isRed)
                            this.clickCoordinates.Remove(point);
                        else
                            point.isRed = true;
                        break;
                    }
                }
                this.Invalidate();
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            const int WIDTH = 20;
            const int HEIGHT = 20;
            if (this.clickCoordinates.Count != 0)
            {
                Lab3_Point prevPoint = (Lab3_Point)this.clickCoordinates[0];
                foreach (Lab3_Point point in this.clickCoordinates)
                {
                    if(point.isRed)
                        e.Graphics.FillEllipse(Brushes.Red, point.coordinates.X - WIDTH / 2, point.coordinates.Y - WIDTH / 2, WIDTH, HEIGHT);
                    else
                        e.Graphics.FillEllipse(Brushes.Black, point.coordinates.X - WIDTH / 2, point.coordinates.Y - WIDTH / 2, WIDTH, HEIGHT);
                    e.Graphics.DrawLine(Pens.Black, point.coordinates, prevPoint.coordinates);
                    prevPoint = point;
                }
            }
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.clickCoordinates.Clear();
            this.Invalidate();
        }
    }

    public class Lab3_Point
    {
        public Point coordinates;
        public bool isRed;
        public Lab3_Point(int x, int y)
        {
            this.coordinates = new Point(x, y);
            this.isRed = false;
        }
    };
}
