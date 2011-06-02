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
        IList<CamperDotNet> campers_;

        public CampAdminGUI()
        {
            try
            {
                InitializeComponent();
                using (new WaitCursor())
                {
                    camp_.Start();
                    campers_ = camp_.getAllCampers();
                    for (int i = 0; i < campers_.Count; ++i) 
                    {
                        CamperDotNet camper = campers_[i];
                        dataGridView.Rows.Add(camper.id_, camper.firstName_, camper.lastName_, camper.amount_);
                    }
                    // [id, first, last, amount] sort by last name
                    dataGridView.Sort(dataGridView.Columns[2], ListSortDirection.Ascending);
                    // disable create button
                    enableCreateButton();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception: " + ex);
            }
        }

        // add a message to the list view, but be careful if called from another thread
        private void Log(string s)
        {
            // id, firstname, lastname, amount
            if (InvokeRequired)
                BeginInvoke(new MethodInvoker(delegate { listView.Items.Add(s); }));
            else
                listView.Items.Add(s);
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

                // Interact with finger reader
                long id = camp_.AddCamper(firstName, lastName, amount);
                // Get the newly enrolled camper
                CamperDotNet camper = camp_.GetCamper(id);
                // Add to DataGridView, sort, and select
                int idx = dataGridView.Rows.Add(camper.id_, camper.firstName_, camper.lastName_, camper.amount_);
                // id, first, last, amount
                DataGridViewCell newCell = dataGridView.Rows[idx].Cells[2];
                dataGridView.Sort(dataGridView.Columns[2], ListSortDirection.Ascending);
                dataGridView.CurrentCell = newCell;
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }

            // after completion (or failure) clear the input
            textBoxFirstName.Text = "";
            textBoxLastName.Text = "";
            maskedTextBoxAmount.Text = "";
        }

        private void bRead_Click(object sender, EventArgs e)
        {
            try
            {
                if (String.IsNullOrEmpty(tReadID.Text))
                    return;
                long id = Convert.ToInt64(tReadID.Text);

                using (new WaitCursor())
                {
                    string description = "";
                    float amount = 0.0f;
//                    camp_.GetCamper(ref id, ref description, ref description, ref amount);
                    Log("Item '" + description + "' read with id " + id);
                }
            }
            catch (CampException ex)
            {
                Log(ex.Message);
            }

            tReadID.Text = "";
        }

        private void bUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                if (String.IsNullOrEmpty(tUpdateID.Text))
                    return;
                if (String.IsNullOrEmpty(tUpdateDesc.Text))
                    return;

                long id = Convert.ToInt64(tUpdateID.Text);
                float amount = Convert.ToSingle(tUpdateDesc.Text);
                camp_.UpdateCamper(id, amount);
                Log("Item " + id + " updated with amount '" + tUpdateDesc.Text + "'");
            }
            catch (CampException ex)
            {
                Log(ex.Message);
            }

            tUpdateID.Text = "";
            tUpdateDesc.Text = "";
        }

        private void bDelete_Click(object sender, EventArgs e)
        {
            try
            {
                if (String.IsNullOrEmpty(tDeleteID.Text))
                    return;

                long id = Convert.ToInt64(tDeleteID.Text);
                camp_.DeleteCamper(id);
                Log("Item " + id + " has been deleted");
            }
            catch (CampException ex)
            {
                Log(ex.Message);
            }

            tDeleteID.Text = "";
        }


        // shut down Camp when the window closes
        private void Client_FormClosing(object sender, FormClosingEventArgs e)
        {
            camp_.Shutdown();
        }

        private void dataGridView_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                DataGridViewCell cell = dataGridView.Rows[e.RowIndex].Cells[0];
                tReadID.Text = Convert.ToString(cell.Value);
            }
        }

        private void textBoxFirstName_Validating(object sender, CancelEventArgs e)
        {
            enableCreateButton();
        }

        private void textBoxLastName_TextChanged(object sender, EventArgs e)
        {
            enableCreateButton();

            if (String.IsNullOrEmpty(textBoxLastName.Text)) return;

            String toFind = textBoxLastName.Text.ToLower();

            for (int i = 0; i < dataGridView.RowCount; ++i)
            {
                // id, first, last, amount
                String lastName = dataGridView.Rows[i].Cells[2].Value.ToString();
                Log(lastName);
                if (lastName.ToLower().StartsWith(toFind))
                {
                    dataGridView.CurrentCell = dataGridView.Rows[i].Cells[2];
                    break;
                }
            }

        }

        private void maskedTextBoxAmount_TextChanged(object sender, EventArgs e)
        {
            enableCreateButton();
        }

        private void maskedTextBoxAmount_Validating(object sender, CancelEventArgs e)
        {
            // Don't be annoying if no value
            if (String.IsNullOrEmpty(maskedTextBoxAmount.Text)) return;

            float num;
            bool isValid = float.TryParse(maskedTextBoxAmount.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out num);
            if (!isValid)
            {
                MessageBox.Show("Invalid Amount, please entry a value amount.");
                e.Cancel = true;
            }
            enableCreateButton();
        }

        private void enableCreateButton()
        {
            float num;
            bool enable = 
                !String.IsNullOrEmpty(textBoxFirstName.Text) &&
                !String.IsNullOrEmpty(textBoxLastName.Text) &&
                float.TryParse(maskedTextBoxAmount.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out num);
            buttonCreate.Enabled = enable;
        }

    }
}