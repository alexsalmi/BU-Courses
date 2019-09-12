using System;
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
    public partial class SettingsDialog : Form
    {
        // Initialize all of the setting variables
        public string fillColor;
        public string penColor;
        public int penSize;
        public bool fill;
        public bool outline;

        public SettingsDialog()
        {
            InitializeComponent();
        }

        // When the settings dialog is opened, update the currently selected iten in each control to match the current settings
        private void SettingsDialog_Load(object sender, EventArgs e)
        {
            penColorBox.SelectedItem = penColor;
            fillColorBox.SelectedItem = fillColor;
            penWidthBox.SelectedItem = penSize.ToString();
            fillButton.Checked = fill;
            outlineButton.Checked = outline;
        }

        // If the user exits by pressing the OK button, update the current setting variables, and set Dialogresult to OK
        private void okButton_Click(object sender, EventArgs e)
        {
            penColor = penColorBox.SelectedItem.ToString();
            fillColor = fillColorBox.SelectedItem.ToString();
            penSize = Convert.ToInt32(penWidthBox.SelectedItem.ToString());
            fill = fillButton.Checked;
            outline = outlineButton.Checked;

            DialogResult = DialogResult.OK;
        }

        // If the user exits by pressing the Cancel button, do not update the current setting variables, and set Dialogresult to cancel
        private void cancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}
