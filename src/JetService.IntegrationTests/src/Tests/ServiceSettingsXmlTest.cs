using System.IO;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class ServiceSettingsXmlTest : SettingsXmlTestBase
  {
    [Test]
    public void Test_Valid()
    {
      DoSettingsTest(new ServiceSettings
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
                       });
    }

    [Test]
    public void Test_Implicit_DisplayName()
    {
      DoSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         Description = "This is a jet service",
                         Execution = new ServiceSettings.ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = "Prog.exe",
                                         WorkDir = null
                                       }
                       });
    }

    [Test]
    public void Test_No_Program()
    {
      DoFailedSettingsTest(new ServiceSettings
                             {
                               Name = "jetService-test",
                               Description = "This is a jet service",
                               Execution = new ServiceSettings.ExecutionElement
                                             {
                                               Arguments = null,
                                               Program = "Not-Existings-Prog.exe",
                                               WorkDir = null
                                             }
                             });
    }

    [Test]
    public void Test_No_Workdir()
    {
      DoFailedSettingsTest(new ServiceSettings
                             {
                               Name = "jetService-test",
                               Description = "This is a jet service",
                               Execution = new ServiceSettings.ExecutionElement
                                             {
                                               Arguments = null,
                                               Program = "Prog.exe",
                                               WorkDir = "a/b/c"
                                             }
                             }, (s, dir) => File.WriteAllText(Path.Combine(dir, s.Execution.Program), "mokkk"));
    }

    [Test]
    public void Test_Execute_Test_Exe()
    {
      DoSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ServiceSettings.ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = Files.TestProgram,
                                         WorkDir = null
                                       }
                       });
    }

    protected override void AssertServiceParameters(ServiceSettings s, JetServiceCommandRunner.Result r, ServiceSettings.ExecutionElement ee)
    {
      var lines = new[]
                    {
                      "[j]: ServiceName:" + s.Name + "!~",
                      "[j]: ServiceDisplayName:" + (s.DisplayName ?? s.Name) + "!~",
                      "[j]: ServiceDescription:" + s.Description + "!~",
                      @"[e]: Program:" + ee.Program + "!~",
                      "[e]: Arguments:" + (ee.Arguments ?? "") + "!~",
                      @"[e]: WorkDir:" + ee.WorkDir + "!~",
                    };

      foreach (var line in lines)
      {
        Assert.IsTrue(r.LogText.Contains(line), "Must contain: {0}", line);
      }
    }

    protected override string ValidateCommandName
    {
      get { return "validate-settings"; }
    }
  }
}
