using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Globalization;


namespace CampPOSNS
{
    public partial class CampAdminGUI : Form
    {
        CampDotNet camp_ = new CampDotNet();
        BackgroundWorker bwCreate = new BackgroundWorker();
        BackgroundWorker bwScan = new BackgroundWorker();

        public CampAdminGUI()
        {
            try
            {
                InitializeComponent();

                bwCreate.DoWork += new DoWorkEventHandler(bwCreate_DoWork);
                bwCreate.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bwCreate_RunWorkerCompleted);

                bwScan.DoWork += new DoWorkEventHandler(bwScan_DoWork);
                bwScan.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bwScan_RunWorkerCompleted);

                using (new WaitCursor())
                {
                    camp_.Start();
                    IList<CamperDotNet> campers = camp_.getAllCampers();
                    for (int i = 0; i < campers.Count; ++i) 
                    {
                        CamperDotNet camper = campers[i];
                        dataGridView.Rows.Add(camper.id_, camper.firstName_, camper.lastName_, camper.amount_.ToString("F"));
                    }
                    if (campers.Count > 0)
                    {
                        // [id, first, last, amount] sort by last name
                        dataGridView.Sort(dataGridView.Columns[2], ListSortDirection.Ascending);
                        dataGridView.Rows[0].Selected = true;
                    }
                    // disable create button
                    enableDisable();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception: " + ex);
            }
        }

        // implement UI method
        private void buttonCreate_Click(object sender, EventArgs e)
        {
            try
            {
                // if input is blank, should be disable, just return
                if (String.IsNullOrEmpty(textBoxFirstName.Text)) return;
                if (String.IsNullOrEmpty(textBoxLastName.Text)) return;
                float amount;
                if (!float.TryParse(maskedTextBoxAmount.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out amount)) return;
                // remove leading/trailing spaces
                String firstName = textBoxFirstName.Text.Trim();
                String lastName = textBoxLastName.Text.Trim();

                CamperDotNet camper = new CamperDotNet();
                camper.firstName_ = firstName;
                camper.lastName_ = lastName;
                camper.amount_ = amount;

                bwCreate.RunWorkerAsync(camper);
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            enableDisable();
        }

        private void bwCreate_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            CamperDotNet camper = (CamperDotNet)e.Argument;

            // Interact with finger reader
            long id = camp_.AddCamper(camper.firstName_, camper.lastName_, camper.amount_);

            e.Result = id;
        }

        private void bwCreate_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            try
            {
                if (e.Error != null)
                {
                    MessageBox.Show(e.Error.Message);
                }
                else if (e.Cancelled)
                {
                    return;
                }
                else
                {
                    long id = (long)e.Result;
                    // Get the newly enrolled camper
                    CamperDotNet camper = camp_.GetCamper(id);
                    // Add to DataGridView, sort, and select
                    int idx = dataGridView.Rows.Add(camper.id_, camper.firstName_, camper.lastName_, camper.amount_);
                    // id, first, last, amount
                    DataGridViewCell newCell = dataGridView.Rows[idx].Cells[2];
                    dataGridView.Sort(dataGridView.Columns[2], ListSortDirection.Ascending);
                    dataGridView.CurrentCell = newCell;
                }
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            // after completion (or failure) clear the input
            textBoxFirstName.Text = "";
            textBoxLastName.Text = "";
            maskedTextBoxAmount.Text = "";
            enableDisable();
        }

        private void buttonScan_Click(object sender, EventArgs e)
        {
            try
            {
                // if no rows, then nothing to scan for
                if (dataGridView.Rows.Count <= 0) return;

                bwScan.RunWorkerAsync();
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            enableDisable();
        }

        private void bwScan_DoWork(object sender, DoWorkEventArgs e)
        {
            // Interact with finger reader
            long id = camp_.FindCamper();

            e.Result = id;
        }

        private void bwScan_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            try
            {
                if (e.Error != null)
                {
                    MessageBox.Show(e.Error.Message);
                }
                else if (e.Cancelled)
                {
                    return;
                }
                else
                {
                    long idToFind = (long)e.Result;
                    for (int i = 0; i < dataGridView.RowCount; ++i)
                    {
                        // id, first, last, amount
                        long id = (long)dataGridView.Rows[i].Cells[0].Value;
                        if (id == idToFind)
                        {
                            dataGridView.CurrentCell = dataGridView.Rows[i].Cells[0];
                            break;
                        }
                    }
                }
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            enableDisable();
        }

        private void buttonUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                if (String.IsNullOrEmpty(textBoxUpdateAmount.Text)) return;
                DataGridViewSelectedRowCollection rows = dataGridView.SelectedRows;
                if (rows.Count <= 0) return;
                float amount;
                bool isValid = float.TryParse(textBoxUpdateAmount.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out amount);

                // id, first, last, amount
                DataGridViewCell idCell = rows[0].Cells[0];
                Int64 id = Convert.ToInt64(idCell.Value);
                camp_.UpdateCamper(id, amount);
                DataGridViewCell amountCell = rows[0].Cells[3];
                amountCell.Value = textBoxUpdateAmount.Text;
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            try
            {
                DataGridViewSelectedRowCollection rows = dataGridView.SelectedRows;
                if (rows.Count > 0)
                {
                    // id, first, last, amount
                    DataGridViewCell cell = rows[0].Cells[0];
                    Int64 id = Convert.ToInt64(cell.Value);
                    camp_.DeleteCamper(id);
                    dataGridView.Rows.Remove(rows[0]);
                }
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            enableDisable();
        }


        // shut down Camp when the window closes
        private void Client_FormClosing(object sender, FormClosingEventArgs e)
        {
            camp_.Shutdown();
        }

        private void dataGridView_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            DataGridViewSelectedRowCollection rows = dataGridView.SelectedRows;
            if (rows.Count > 0)
            {
                // id, first, last, amount
                DataGridViewCell cell = rows[0].Cells[3];
                textBoxUpdateAmount.Text = Convert.ToSingle(cell.Value).ToString("F");
            }
            else
            {
                textBoxUpdateAmount.Text = "";
            }
            enableDisable();
        }

        private void textBoxFirstName_Validating(object sender, CancelEventArgs e)
        {
            enableDisable();
        }

        private void textBoxLastName_TextChanged(object sender, EventArgs e)
        {
            enableDisable();

            if (String.IsNullOrEmpty(textBoxLastName.Text)) return;

            String toFind = textBoxLastName.Text.ToLower();

            for (int i = 0; i < dataGridView.RowCount; ++i)
            {
                // id, first, last, amount
                String lastName = dataGridView.Rows[i].Cells[2].Value.ToString();
                if (lastName.ToLower().StartsWith(toFind))
                {
                    dataGridView.CurrentCell = dataGridView.Rows[i].Cells[2];
                    break;
                }
            }
        }

        private void maskedTextBoxAmount_TextChanged(object sender, EventArgs e)
        {
            enableDisable();
        }

        private void maskedTextBoxAmount_Validating(object sender, CancelEventArgs e)
        {
            // Don't be annoying if no value
            if (String.IsNullOrEmpty(maskedTextBoxAmount.Text)) return;

            float amount;
            bool isValid = float.TryParse(maskedTextBoxAmount.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out amount);
            if (!isValid)
            {
                MessageBox.Show("Invalid Amount, please entry a value amount.");
                e.Cancel = true;
            }
            enableDisable();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            camp_.CancelOperation();
        }

        private void enableDisable()
        {
            // Create/Scan button
            float num;
            bool enable =
                !String.IsNullOrEmpty(textBoxFirstName.Text) &&
                !String.IsNullOrEmpty(textBoxLastName.Text) &&
                float.TryParse(maskedTextBoxAmount.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out num);
            buttonCreate.Enabled = enable;

            // Cancel button
            bool bwBusy = bwCreate.IsBusy || bwScan.IsBusy;
            buttonCancel.Enabled = bwBusy;
            // Progrss bar
            if (bwBusy)
            {
                progressBarScan.Style = ProgressBarStyle.Marquee;
                if (bwCreate.IsBusy)
                {
                    progressBarScan.MarqueeAnimationSpeed = 300;
                    labelProgress.Text = "Scan Finger again.";
                }
                else
                {
                    progressBarScan.MarqueeAnimationSpeed = 100;
                    labelProgress.Text = "Scan Finger.";
                }
                labelProgress.ForeColor = System.Drawing.Color.Red;
                labelProgress.Update();
            }
            else
            {
                progressBarScan.Style = ProgressBarStyle.Blocks;
                labelProgress.Text = "Ready.";
                labelProgress.ForeColor = System.Drawing.Color.Black;
                labelProgress.Update();
            }
            // Delete, Update buttons
            bool selectedRow = dataGridView.SelectedRows.Count > 0;
            buttonDelete.Enabled = selectedRow;
            buttonUpdate.Enabled = selectedRow;
            // Find/Scan button
            buttonScan.Enabled = dataGridView.Rows.Count > 0;
        }

    }
}