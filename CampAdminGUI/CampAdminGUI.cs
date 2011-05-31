using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


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
                MessageBox.Show("Starting");
                using (new WaitCursor())
                {
                    camp_.Start();
                    campers_ = camp_.getAllCampers();
                    for (int i = 0; i < campers_.Count; ++i) 
                    {
                        CamperDotNet camper = campers_[i];
                        dataGridView.Rows.Add(camper.id_, camper.firstName_, camper.lastName_, camper.amount_);
                    }
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
        private void bCreate_Click(object sender, EventArgs e)
        {
            try
            {
                // if input is blank, do nothing, else create the item
                if (String.IsNullOrEmpty(tCreateDesc.Text))
                    return;

                // invoke the method
                long id = camp_.AddCamper(tCreateDesc.Text, tCreateDesc.Text, 25.0f);
                Log("Item '" + tCreateDesc.Text + "' created with id " + id);
            }
            catch (CampException ex)
            {
                Log(ex.Message);
            }

            // after completion (or failure) clear the input
            tCreateDesc.Text = "";
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
                    camp_.GetCamper(ref id, ref description, ref description, ref amount);
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
            DataGridViewCell cell = dataGridView.Rows[e.RowIndex].Cells[0];
            tReadID.Text = Convert.ToString(cell.Value);
        }

    }
}