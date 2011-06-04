// Copyright 2011 Kevin Heifner.  All rights reserved.
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace CampPOSNS
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the CampAdmin GUI application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                if (ProcessUtil.IsProcessOpen("CampSale"))
                {
                    MessageBox.Show("Please close CampSale first.");
                    return;
                }
                if (ProcessUtil.IsProcessOpen("CampAdmin"))
                {
                    MessageBox.Show("CampAdmin already running.");
                    return;
                }

                Application.Run(new CampAdminGUI());
            }
            catch (Exception e)
            {
                try
                {
                    MessageBox.Show(e.Message, "Camp Admin");
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