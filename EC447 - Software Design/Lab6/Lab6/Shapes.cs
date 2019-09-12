using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab6
{
    // Base class 'Shapes'
    class Shapes
    {
        // Variables needed by multiple subclasses
        public bool fill;
        public bool outline;
        public Pen pen;
        public Rectangle area;
        public Brush brush;

        // Virtual Draw() function to be overriden by subclasses
        public virtual void Draw(Graphics g){}
    }

    // Subclass 'Line'
    class Line : Shapes
    {
        // Point variables only used by Line, to draw the line
        public Point point1;
        public Point point2;

        // Constructor - Set the two points for the line and the pen to use given the inputs
        public Line(string penColor, int penSize, Point p1, Point p2)
        {
            point1 = p1;
            point2 = p2;
            
            pen = new Pen(Color.FromName(penColor), penSize);
        }

        // Draw the line using the pen and two points
        public override void Draw(Graphics g)
        {
            g.DrawLine(pen, point1, point2);
        }
    }

    class Rect : Shapes
    {
        // Constructor - Set the pen and brush color, as well as the bounding rectangle and whether we should draw a fill or outline
        public Rect(string penColor, string fillColor, int penSize, Point point, Size size, bool Fill, bool Outline)
        {
            fill = Fill;
            outline = Outline;
            pen = new Pen(Color.FromName(penColor), penSize);

            if (fillColor == "White")
                brush = Brushes.White;
            else if (fillColor == "Black")
                brush = Brushes.Black;
            else if (fillColor == "Red")
                brush = Brushes.Red;
            else if (fillColor == "Green")
                brush = Brushes.Green;
            else if (fillColor == "Blue")
                brush = Brushes.Blue;

            area = new Rectangle(point, size);
        }

        // Draw the rectangle using the brush/pen, based on the bounding rectangle
        // Draw the fill or outline or both, depending on bool variables. Outline comes after fill to make sure it's visible
        public override void Draw(Graphics g)
        {
            if (fill)
                g.FillRectangle(brush, area);
            if (outline)
                g.DrawRectangle(pen, area);
        }
    }

    class Ellipse : Shapes
    {
        // Constructor - Set the pen and brush color, as well as the bounding rectangle and whether we should draw a fill or outline
        public Ellipse(string penColor, string fillColor, int penSize, Point point, Size size, bool Fill, bool Outline)
        {
            fill = Fill;
            outline = Outline;
            pen = new Pen(Color.FromName(penColor), penSize);

            if (fillColor == "White")
                brush = Brushes.White;
            else if (fillColor == "Black")
                brush = Brushes.Black;
            else if (fillColor == "Red")
                brush = Brushes.Red;
            else if (fillColor == "Green")
                brush = Brushes.Green;
            else if (fillColor == "Blue")
                brush = Brushes.Blue;

            area = new Rectangle(point, size);
        }

        // Draw the ellipse using the brush/pen, based on the bounding rectangle
        // Draw the fill or outline or both, depending on bool variables. Outline comes after fill to make sure it's visible
        public override void Draw(Graphics g)
        {
            if (fill)
                g.FillEllipse(brush, area);
            if(outline)
                g.DrawEllipse(pen, area);
        }
    }
}
