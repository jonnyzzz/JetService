using System;
using System.IO;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class ServiceSettingsXmlTest
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

    private static void DoFailedSettingsTest(ServiceSettings s, Action<ServiceSettings, string> layout = null)
    {
      var r = DoRawSettingsTest(s, layout);
      Assert.That(r.ExitCode, Is.Not.EqualTo(0));
    }


    private static JetServiceCommandRunner.Result DoSettingsTest(ServiceSettings s)
    {
      Func<string, ServiceSettings.ExecutionElement, ServiceSettings.ExecutionElement> resolve =
        (dir, e) => new ServiceSettings.ExecutionElement
                      {
                        Arguments = e.Arguments,
                        Program = Path.Combine(dir, e.Program),
                        WorkDir = e.WorkDir == null ? dir : Path.Combine(dir, e.WorkDir),
                      };

     return TempFilesHolder.WithTempDirectory(
        dir =>
          {
            var ee = resolve(dir, s.Execution);
            File.WriteAllText(ee.Program, "mock");

            var r = ExecuteWithSettings(s, dir);
            Assert.That(r.ExitCode, Is.EqualTo(0));

            var lines = new[]
                          {
                            "[j]: ServiceName:" + s.Name + "!~",
                            "[j]: ServiceDisplayName:" + (s.DisplayName??s.Name) + "!~",
                            "[j]: ServiceDescription:" + s.Description + "!~",
                            @"[e]: Program:" + ee.Program + "!~",
                            "[e]: Arguments:" + (ee.Arguments ?? "") + "!~",
                            @"[e]: WorkDir:" + ee.WorkDir + "!~",
                          };

            foreach (var line in lines)
            {
              Assert.IsTrue(r.LogText.Contains(line), "Must contain: {0}", line);
            }
            return r;
          });
    }

    private static JetServiceCommandRunner.Result DoRawSettingsTest(ServiceSettings s, Action<ServiceSettings, string> layout)
    {
      return TempFilesHolder.WithTempDirectory(
        dir =>
          {
            if (layout != null) layout(s, dir);
            return ExecuteWithSettings(s, dir);
          });
    }

    private static JetServiceCommandRunner.Result ExecuteWithSettings(ServiceSettings s, string dir)
    {
      var settings = Path.Combine(dir, "settings.xml");
      s.Serialize(settings);
      Console.Out.WriteLine("Settings: {0}", s);

      var r = JetServiceCommandRunner.ExecuteCommand("validate-settings", "/settings=" + settings);
      Console.Out.WriteLine(r.LogText);
      return r;
    }
  }
};