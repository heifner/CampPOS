using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace CampPOSNS
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                if (IsProcessOpen("CampAdmin"))
                {
                    MessageBox.Show("Please close CampAdmin first.");
                    return;
                }
                if (IsProcessOpen("CampSale"))
                {
                    MessageBox.Show("CampSale already running.");
                    return;
                }
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new CampSaleGUI());
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
        }

        /// <returns>true if process with given name found and not current process</returns>
        static public bool IsProcessOpen(string name)
        {
            int currentProcessId = Process.GetCurrentProcess().Id;
            // List of all running processes on computer
            foreach (Process clsProcess in Process.GetProcesses())
            {
                if (clsProcess.Id != currentProcessId && clsProcess.ProcessName.Contains(name)) return true;
            }
            return false;
        }
    }
}