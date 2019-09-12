using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab4
{
    // This form is a solution to the classic eight queens puzzle, where a chess board must be filled
    // with eight queens in such a way that no queen is on the same line as another. 
    // They can not be on the same row or column of the board as another queen, and cannot be 
    // on the same diagonal as another queen. 
    // This form works by allowing the user to choose which row to place a queen in the first column, 
    // and then the program calculates a valid solution (the locations of the rest of the queens) based on
    // the chosen first queen position
    // http://en.wikipedia.org/wiki/Eight_queens_puzzle.
    public partial class Form1 : Form
    {
        // A bool matrix to represent the chess board (true means queen is located in this square)
        private bool[,] board = new bool[8, 8];
        private Font myfont;

        // This is a recursive function to iterate through columns 1-7, finding valid positions for the queens 
        // based on the columns before the current one
        private bool PlaceQueen(int col)
        {   
            // Iterate through the rows in the current column
            for (int row = 0; row <= 7; ++row)
            {
                // Check if the current square is a valid position
                if (IsSafe(col, row))
                {
                    board[col, row] = true; //place queen

                    // If we are on column 7, we are done
                    if (col == 7)
                        return true;    //we have a solution
                    // Otherwise, continue by continueing the recursive call on the next column
                    else
                    {
                        // If PlaceQueen returns true, we can continue to the next column, else we must remove the queen from the current square
                        if (PlaceQueen(col + 1))    //continue to next column
                            return true;
                        else
                            board[col, row] = false;   //retract move and look for another safe square
                    }
                }
            }
            return false;   //no safe columns left so backtrack
        }
        
        // Checks if a given squre is a valid location for a queen based on the columns to the left of it
        private bool IsSafe(int col, int row)
        {
            int r, c;
            // Check for any queens on the same row
            for(c=0; c<col; c++)
                if (board[c, row])
                    return false;

            // Check for queens on the diagonal
            r = row - 1;
            c = col - 1;
            while(c>=0 && r >= 0)
            {
                if (board[c, r])
                    return false;
                c--;
                r--;
            }
            r = row + 1;
            c = col - 1;
            while (c >= 0 && r <= 7)
            {
                if (board[c, r])
                    return false;
                c--;
                r++;
            }


            // If we have made it past the loops, we are safe
            return true;
        }

        public Form1()
        {
            InitializeComponent();
            this.Text = "Eight Queens by Alexander Salmi";
            myfont = new Font("Arial", 30, FontStyle.Bold);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = e.Graphics;

            bool isBlack = false;   // Keep track of tile color
            Rectangle rect; // Tile location
            StringFormat sf = new StringFormat();   // To center Q in tiles
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            // Draw board
            for (int col = 0; col < 8; col++)
            {
                for (int row = 0; row < 8; row++)
                {
                    // Size/location of each square
                    rect = new Rectangle(100 + (50 * col), 100 + (50 * row), 50, 50);

                    if (isBlack)
                    {
                        g.FillRectangle(Brushes.Black, rect);
                        if (board[col, row])
                            g.DrawString("Q", myfont, Brushes.White, rect, sf);
                    }
                    else
                    {
                        g.FillRectangle(Brushes.White, rect);
                        if (board[col, row])
                            g.DrawString("Q", myfont, Brushes.Black, rect, sf);
                    }

                    g.DrawRectangle(Pens.Black, rect);
                    isBlack = !isBlack;
                }
                isBlack = !isBlack;
            }
        }

        private void FindSolution_Click(object sender, EventArgs e)
        {
            board = new bool[8, 8]; //clear board
            //Set row in column 0 from numeric up/down control
            board[0, (int)StartRow.Value] = true;
            PlaceQueen(1);  //start at column 1
            Invalidate();   //draw board
        }
    }
}
