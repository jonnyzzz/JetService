using System;
using System.IO;
using System.Linq;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public class InstallRemoveServiceBase
  {
    protected string[] A(params string[] str)
    {
      return str;
    }

    protected void InstallRemoveService(string[] installServiceArguments,
                                        TestAction action, 
                                        string[] testProgramArguments,
                                        Action<ServiceSettings, string> afterInstalled)
    {
      ServicesUtil.AssertHasInstallServiceRights();

      TempFilesHolder.WithTempDirectory(
        dir =>
        {
          var hash = Guid.NewGuid().ToString();
          var settingsXml = new ServiceSettings
                              {
                                Name = "jetService-test-" + hash,
                                Description = "This is a jet service " + hash,
                                Execution = new ServiceSettings.ExecutionElement
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

          Assert.IsTrue(IsServiceInstalled(settingsXml), "Service must be installed: {0}", settingsXml.Name);

          try
          {
            afterInstalled(settingsXml, dir);
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
            
            Assert.IsFalse(IsServiceInstalled(settingsXml), "Service must be uninstalled: {0}", settingsXml.Name);
          }
        });
    }

    private static bool IsServiceInstalled(ServiceSettings settingsXml)
    {
      var allServices = ServicesUtil.ListServices();
      return allServices.Select(x => x.ToLower()).Contains(settingsXml.Name.ToLower());
    }
  }
}