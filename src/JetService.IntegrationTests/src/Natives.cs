using System;
using System.Runtime.InteropServices;

namespace JetService.IntegrationTests
{
  public class Natives
  {
    [DllImport("advapi32.dll", EntryPoint = "OpenSCManagerW", ExactSpelling = true, CharSet = CharSet.Unicode, SetLastError = true)]
    public static extern IntPtr OpenSCManager(string machineName, string databaseName, SCM_ACCESS dwAccess);

    [DllImport("advapi32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static extern bool CloseServiceHandle(IntPtr hSCObject);

    [Flags]
    public enum SCM_ACCESS : uint
    {
      SC_ALL_ACCESS = 983103,
      CREATE_SERVICE = 2,
      ENUMERATE_SERVICE = 4,
    }
  }
}
