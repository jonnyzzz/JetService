using System.Collections.Generic;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class CreateServiceSettingsXmlTest : SettingsXmlTestBase
  {
    public ServiceSettings DefaultSettins
    {
      get
      {
        return new ServiceSettings
                 {
                   Name = "jetService-test",
                   DisplayName = "Jet Service Test",
                   Description = "This is a jet service",
                   Execution = new ServiceSettings.ExecutionElement
                                 {
                                   Arguments = null,
                                   Program = "Prog.exe",
                                   WorkDir = null
                                 }
                 };
      }
    }

    private class ServiceInfo
    {
      public bool IsAutostart { get; set; }
      public bool RunAsService { get; set; }
      public string UserName { get; set; }
      public string Password { get; set; }
      public string Domain { get; set; }
    }

    [Test]
    public void Test_Valid_System()
    {
      DoSettingsTest(
        new ServiceInfo
          {
            IsAutostart = true,
            RunAsService = true,
          },
        DefaultSettins, "/runAsSystem");
    }

    [Test]
    public void Test_Valid_System_autostart()
    {
      DoSettingsTest(
        new ServiceInfo
          {
            IsAutostart = false,
            RunAsService = true,
          },
        DefaultSettins, "/runAsSystem", "/autostart=false");
    }

    [Test]
    public void Test_Valid_User()
    {
      DoSettingsTest(
        new ServiceInfo
          {
            IsAutostart = true,
            RunAsService = false,
            UserName = "User444",
            Password = "Pass554",
          },
          DefaultSettins, "/user=User444", "/password=Pass554");
    }

    [Test]
    public void Test_Valid_UserDomain()
    {
      DoSettingsTest(
        new ServiceInfo
          {
            IsAutostart = true,
            RunAsService = false,
            UserName = "User444",
            Password = "Pass554",
            Domain= ".",
          },
          DefaultSettins, "/user=User444", "/password=Pass554", "/domain=.");
    }

    [Test]
    public void Test_Valid_User_autostart()
    {
      DoSettingsTest(
        new ServiceInfo
          {
            IsAutostart = false,
            RunAsService = false,
            UserName = "User444",
            Password = "Pass554",
          },
          DefaultSettins, "/user=User444", "/password=Pass554", "/autostart=false");
    }

    protected override void AssertServiceParameters(ServiceSettings s, JetServiceCommandRunner.Result r, ServiceSettings.ExecutionElement ee)
    {
      var lines = new List<string>
                    {
                      "[j]: ServiceName:" + s.Name + "!~",
                      "[j]: ServiceDisplayName:" + (s.DisplayName ?? s.Name) + "!~",
                      "[j]: ServiceDescription:" + s.Description + "!~",
                      @"[e]: Program:" + ee.Program + "!~",
                      "[e]: Arguments:" + (ee.Arguments ?? "") + "!~",
                      @"[e]: WorkDir:" + ee.WorkDir + "!~",
                      "[j]: IsAutostart:" + myInfo.IsAutostart.ToString().ToLower() + "!~",
                      "[j]: RunAsService:" + myInfo.RunAsService.ToString().ToLower() + "!~",
                    };
      if (!myInfo.RunAsService)
      {
        lines.Add("[j]: UserName:" + myInfo.UserName + "!~");
        lines.Add("[j]: Password:" + myInfo.Password + "!~");
        lines.Add("[j]: Domain:" + (myInfo.Domain ?? ".") + "!~");
      } else
      {
        foreach (var line in new[] { "[j]: Password:", "[j]: UserName:" })
        {
          Assert.IsFalse(r.LogText.Contains(line), "Must not contain: {0}", line);
        }
      }
      foreach (var line in lines)
      {
        Assert.IsTrue(r.LogText.Contains(line), "Must contain: {0}", line);
      }
    }

    protected override string ValidateCommandName
    {
      get { return "validate-create-settings"; }
    }

    private ServiceInfo myInfo = null;

    [SetUp]
    public void SetUp()
    {
      myInfo = null;
    }

    private void DoSettingsTest(ServiceInfo info, ServiceSettings s, params string[] cmd)
    {
      myInfo = info;
      DoSettingsTest(s, cmd);
      myInfo = null;
    }
  }  
}