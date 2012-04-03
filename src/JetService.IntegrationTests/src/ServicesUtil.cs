using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public static class ServicesUtil
  {
    public static void AssertHasInstallServiceRights()
    {
      IntPtr scm = Natives.OpenSCManager(null, null, Natives.SCM_ACCESS.CREATE_SERVICE | Natives.SCM_ACCESS.ENUMERATE_SERVICE);
      if (scm == IntPtr.Zero)
      {
        Assert.Ignore("Not enough righst to install service");
        return;
      }
      Natives.CloseHandle(scm);
    }

    public static IEnumerable<string> ListServices()
    {
      const string ns = @"root\cimv2";
      const string query = "select * from Win32_Service";

      var scope = new ManagementScope(string.Format(@"\\{0}\{1}", Environment.MachineName, ns));
      scope.Connect();

      using (var searcher = new ManagementObjectSearcher(scope, new ObjectQuery(query)))
      {
        return searcher.Get().Cast<ManagementObject>().Select(x => x["Name"].ToString()).ToArray();
      }
    }

  }
}