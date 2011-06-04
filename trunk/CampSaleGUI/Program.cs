// Copyright 2011 Kevin Heifner.  All rights reserved.
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace CampPOSNS
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the CampSale GUI application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                if (ProcessUtil.IsProcessOpen("CampAdmin"))
                {
                    MessageBox.Show("Please close CampAdmin first.", "Camp Sale");
                    return;
                }
                if (ProcessUtil.IsProcessOpen("CampSale"))
                {
                    MessageBox.Show("CampSale already running.", "Camp Sale");
                    return;
                }

                Application.Run(new CampSaleGUI());
            }
            catch (Exception e)
            {
                try
                {
                    MessageBox.Show(e.Message, "Camp Sale");
                }
                catch (Exception) 
                {
                    // If unable to show message box then rethrow original exception
                    throw e;
                }
                // Application.Exit() and Environment.Exit(1) didn't work.
                Process.GetCurrentProcess().Kill();
            }
        }
    }
}