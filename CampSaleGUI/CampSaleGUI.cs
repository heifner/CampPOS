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
    public partial class CampSaleGUI : Form
    {
        CampDotNet camp_ = new CampDotNet();
        BackgroundWorker bwScan = new BackgroundWorker();
        CamperDotNet currentCamper_;

        public CampSaleGUI()
        {
            try
            {
                InitializeComponent();

                bwScan.DoWork += new DoWorkEventHandler(bwScan_DoWork);
                bwScan.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bwScan_RunWorkerCompleted);

                using (new WaitCursor())
                {
                    camp_.Start();
                    textBoxPrice.Text = camp_.GetDefaultPrice().ToString("F");
                    // disable create button
                    enableDisable();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception: " + ex);
            }
        }

        private void buttonScan_Click(object sender, EventArgs e)
        {
            scanFinger();
        }

        private void scanFinger()
        {
            currentCamper_.id_ = 0;
            textBoxFirstName.Text = "";
            textBoxLastName.Text = "";
            maskedTextBoxAmount.Text = "";

            textBoxQuantity.Focus();
            textBoxQuantity.SelectAll();

            try
            {
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
                currentCamper_.id_ = 0;
                textBoxFirstName.Text = "";
                textBoxLastName.Text = "";
                maskedTextBoxAmount.Text = "";

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
                    currentCamper_.id_ = (long)e.Result;
                    currentCamper_ = camp_.GetCamper(currentCamper_.id_);
                    textBoxFirstName.Text = currentCamper_.firstName_;
                    textBoxLastName.Text = currentCamper_.lastName_;
                    maskedTextBoxAmount.Text = currentCamper_.amount_.ToString("F");
                }
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            enableDisable();
        }

        private void buttonPurchase_Click(object sender, EventArgs e)
        {
            try
            {
                if (currentCamper_.id_ == 0) return;
                if (String.IsNullOrEmpty(textBoxPrice.Text)) return;
                if (String.IsNullOrEmpty(textBoxQuantity.Text)) return;

                float price;
                bool priceIsValid = float.TryParse(textBoxPrice.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out price);
                int quantity;
                bool quantityIsValid = int.TryParse(textBoxQuantity.Text, out quantity);
                if (!priceIsValid || !quantityIsValid)
                {
                    MessageBox.Show("Please enter a valid Price & Quantity.");
                    return;
                }

                // update database
                float totalPrice = quantity * price;
                float newAmount = currentCamper_.amount_ - totalPrice;
                if (newAmount < 0.0)
                {
                    MessageBox.Show("Unable to purchase, not enough in account");
                    return;
                }
                camp_.UpdateCamper(currentCamper_.id_, newAmount);
                
                // report in transaction list
                String msg = currentCamper_.firstName_ + " " + currentCamper_.lastName_ +
                    " purchased " + quantity.ToString() + " items for a total of $" + totalPrice.ToString("F") +
                    ", remaining amount $" + newAmount.ToString("F");
                listBoxTransactions.Items.Insert(0, msg);

                // scan next finger
                scanFinger();
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

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            try
            {
                camp_.CancelOperation();
            }
            catch (CampException ex)
            {
                MessageBox.Show(ex.Message);
            }
            enableDisable();
        }

        private void textBoxQuantity_Leave(object sender, EventArgs e)
        {
            enableDisable();
        }

        private void textBoxPrice_Leave(object sender, EventArgs e)
        {
            enableDisable();
        }

        private void enableDisable()
        {
            // Cancel button
            bool bwBusy = bwScan.IsBusy;
            buttonCancel.Enabled = bwBusy;
            // Scan button
            buttonScan.Enabled = !bwBusy;
            // Purchase button
            buttonPurchase.Enabled = currentCamper_.id_ != 0 &&
                !String.IsNullOrEmpty(textBoxPrice.Text) &&
                !String.IsNullOrEmpty(textBoxQuantity.Text);

            // Progrss bar
            if (bwBusy)
            {
                progressBarScan.Style = ProgressBarStyle.Marquee;
                progressBarScan.MarqueeAnimationSpeed = 100;
                labelProgress.Text = "Scan Finger.";
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
        }

    }
}