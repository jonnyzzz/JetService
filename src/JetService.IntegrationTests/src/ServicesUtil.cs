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
      Natives.CloseServiceHandle(scm);
    }

    public static IEnumerable<ServiceInfo> ListServices()
    {
      const string ns = @"root\cimv2";
      const string query = "select * from Win32_Service";

      var scope = new ManagementScope(string.Format(@"\\{0}\{1}", Environment.MachineName, ns));
      scope.Connect();

      using (var searcher = new ManagementObjectSearcher(scope, new ObjectQuery(query)))
      {
        return searcher
          .Get().Cast<ManagementObject>()
          .Select(x => new ServiceInfo
                         {
                           Name = x["Name"].ToString(),
                           Status = x["State"].ToString(),
                         })
          .ToArray();
      }
    }

    public static void RemoveServices(Func<string, bool> predicate)
    {
      const string ns = @"root\cimv2";
      const string query = "select * from Win32_Service";

      var scope = new ManagementScope(string.Format(@"\\{0}\{1}", Environment.MachineName, ns));
      scope.Connect();

      using (var searcher = new ManagementObjectSearcher(scope, new ObjectQuery(query)))
      {        
        foreach (ManagementObject mo in searcher.Get())
        {
          string name = mo["Name"].ToString();
          if (predicate(name))
          {
            Console.Out.WriteLine("Found leaked service to remove: {0}", name);
            mo.InvokeMethod("delete", null);    
            mo.Delete();            
          }
        }        
      }      
    }

    public static ProcessExecutor.Result StartService(ServiceSettings s)
    {
      return ProcessExecutor.ExecuteProcess("net.exe", "start " + s.Name).Dump();
    }

    public static ProcessExecutor.Result StopService(ServiceSettings s)
    {
      return ProcessExecutor.ExecuteProcess("net.exe", "stop " + s.Name).Dump();
    }

    public static bool IsServiceNotStopped(ServiceSettings s)
    {
      return ListServices().Where(x => x.IsNamed(s.Name)).Any(x => !x.IsStopped);
    }

    public static bool IsServiceRunning(ServiceSettings s)
    {
      return ListServices().Where(x => x.IsNamed(s.Name)).Any(x => x.Status == "Running");
    }

    public static bool IsServiceInstalled(ServiceSettings settingsXml)
    {
      return ListServices().Any(x => x.IsNamed(settingsXml.Name));
    }

  }
}