// Copyright 2011 Kevin Heifner.  All rights reserved.
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
            // We want an exception thrown if anything goes wrong, so that Program can exit.
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

        private void showMessageBox(String msg)
        {
            MessageBoxEx.Show(this, msg, "Camp Sale");
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
            catch (Exception ex)
            {
                showMessageBox(ex.Message);
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
                    showMessageBox(e.Error.Message);
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
            catch (Exception ex)
            {
                showMessageBox(ex.Message);
            }
            enableDisable();
        }

        private bool validateQuantityPrice(out int quantity, out float price)
        {
            quantity = 0;
            price = 0.0f;
            if (currentCamper_.id_ == 0) return false;
            if (String.IsNullOrEmpty(textBoxPrice.Text)) return false;
            if (String.IsNullOrEmpty(textBoxQuantity.Text)) return false;

            bool priceIsValid = float.TryParse(textBoxPrice.Text, NumberStyles.Currency, CultureInfo.GetCultureInfo("en-US"), out price);
            bool quantityIsValid = int.TryParse(textBoxQuantity.Text, out quantity);
            if (!priceIsValid || !quantityIsValid)
            {
                showMessageBox("Please enter a valid Quantity & Price.");
                return false;
            }
            return true;
        }

        private void buttonPurchase_Click(object sender, EventArgs e)
        {
            try
            {
                int quantity;
                float price;
                if (!validateQuantityPrice(out quantity, out price)) return;

                // update database
                float totalPrice = quantity * price;
                float newAmount = currentCamper_.amount_ - totalPrice;
                if (newAmount < 0.0)
                {
                    showMessageBox("Unable to purchase, not enough in account.");
                    return;
                }
                camp_.UpdateCamper(currentCamper_.id_, newAmount);
                
                // report in transaction list
                String msg = currentCamper_.firstName_ + " " + currentCamper_.lastName_ +
                    " purchased " + quantity.ToString() + " items for a total of $" + totalPrice.ToString("F") +
                    ", remaining account balance $" + newAmount.ToString("F");
                listBoxTransactions.Items.Insert(0, msg);

                // scan next finger
                scanFinger();
            }
            catch (Exception ex)
            {
                showMessageBox(ex.Message);
            }
            enableDisable();
        }

        private void buttonRefund_Click(object sender, EventArgs e)
        {
            try
            {
                int quantity;
                float price;
                if (!validateQuantityPrice(out quantity, out price)) return;

                // update database
                float totalPrice = quantity * price;
                float newAmount = currentCamper_.amount_ + totalPrice;
                camp_.UpdateCamper(currentCamper_.id_, newAmount);

                // report in transaction list
                String msg = currentCamper_.firstName_ + " " + currentCamper_.lastName_ +
                    " was refunded " + quantity.ToString() + " items for a total of $" + totalPrice.ToString("F") +
                    ", new account balance $" + newAmount.ToString("F");
                listBoxTransactions.Items.Insert(0, msg);

                // scan next finger
                scanFinger();
            }
            catch (Exception ex)
            {
                showMessageBox(ex.Message);
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
            catch (Exception ex)
            {
                showMessageBox(ex.Message);
            }
            enableDisable();
        }

        private void textBoxQuantity_TextChanged(object sender, EventArgs e)
        {
            enableDisable();
        }

        private void textBoxQuantity_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.buttonPurchase.PerformClick();
            }
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
            // Purchase, Refund buttons
            bool validPurchaseOrRefund = currentCamper_.id_ != 0 &&
                !String.IsNullOrEmpty(textBoxPrice.Text) &&
                !String.IsNullOrEmpty(textBoxQuantity.Text);
            buttonPurchase.Enabled = validPurchaseOrRefund;
            buttonRefund.Enabled = validPurchaseOrRefund;
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