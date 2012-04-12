using System;
using System.IO;
using System.Linq;
using System.Threading;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public class InstallRemoveServiceBase
  {
    protected void StartStopService(ServiceSettings service, string log, Action afterStarted) {
      try
      {
        try
        {
          ServicesUtil.StartService(service).AssertExitedSuccessfully();
        } finally
        {
          if (log != null)
          {
            using(var s = new FileStream(log, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            using(var r = new StreamReader(s))
            Console.Out.WriteLine("Start service log:\r\n{0}", r.ReadToEnd());
          }
        }
        afterStarted();
      } catch(Exception e)
      {
        Console.Out.WriteLine("Failed " + e);
        throw;
      } finally
      {
        ServicesUtil.StopService(service).AssertExitedSuccessfully();
      }
    }

    protected delegate void OnServiceInstalled(ServiceSettings setting, string dir, string logFile);

    protected void InstallRemoveService(string[] installServiceArguments,
                                        TestAction action, 
                                        string[] testProgramArguments,
                                        OnServiceInstalled afterInstalled)
    {
      ServicesUtil.AssertHasInstallServiceRights();

      TempFilesHolder.WithTempDirectory(
        dir =>
        {
          var hash = (int)(DateTime.Now - new DateTime(2012, 04, 01)).TotalMilliseconds % 9999;
          var settingsXml = new ServiceSettings
                              {
                                Name = "jetService-test-" + hash,
                                Description = "This is a jet service " + hash,
                                Execution = new ExecutionElement
                                              {
                                                Arguments = action + " " + string.Join(" ", testProgramArguments),
                                                Program = Files.TestProgram,
                                                WorkDir = dir
                                              }
                              };
          var settings = Path.Combine(dir, "settings.xml");
          settingsXml.Serialize(settings);
          Console.Out.WriteLine("Settings: {0}", settingsXml);


          var r = JetServiceCommandRunner.ExecuteCommand("install", "/settings=" + settings + " " + string.Join(" ", installServiceArguments));
          Console.Out.WriteLine(r.LogText);
          r.AssertSuccess();


          Thread.Sleep(TimeSpan.FromSeconds(1));
          Assert.IsTrue(IsServiceInstalled(settingsXml), "Service must be installed: {0}", settingsXml.Name);

          try
          {
            afterInstalled(settingsXml, dir, r.LogFilePath);
          }
          catch (Exception e)
          {
            Console.Out.WriteLine("Failed: " + e.Message + e);
            throw;
          }
          finally
          {
            r = JetServiceCommandRunner.ExecuteCommand("delete", "/settings=" + settings);
            Console.Out.WriteLine(r.LogText);
            r.AssertSuccess();

            WaitFor.WaitForAssert(() => IsServiceInstalled(settingsXml), "Service must be uninstalled: {0}", settingsXml.Name);
          }
        });
    }

    [TestFixtureTearDown]
    public void FixtureTearDown()
    {
      try
      {
        ServicesUtil.RemoveServices(x => x.ToLower().StartsWith("jetService-test".ToLower()));
      } catch(Exception e) 
      {
        Console.Out.WriteLine("Failed to cleanup services. {0}", e);
      }
    }

    private static bool IsServiceInstalled(ServiceSettings settingsXml)
    {
      var allServices = ServicesUtil.ListServices();
      return allServices.Select(x => x.ToLower()).Contains(settingsXml.Name.ToLower());
    }
  }
}