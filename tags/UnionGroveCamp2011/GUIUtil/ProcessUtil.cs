using System.Diagnostics;

static public class ProcessUtil
{
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