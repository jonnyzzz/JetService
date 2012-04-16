using System.Collections.Generic;
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
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
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
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
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
      DoRawFailedSettingsTest(new ServiceSettings
                             {
                               Name = "jetService-test",
                               Description = "This is a jet service",
                               Execution = new ExecutionElement
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
      DoRawFailedSettingsTest(new ServiceSettings
                             {
                               Name = "jetService-test",
                               Description = "This is a jet service",
                               Execution = new ExecutionElement
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
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = "ppp.exe",
                                         WorkDir = null
                                       }
                       });
    }

    [Test]
    public void Test_Execute_Test_Exe_Timeout()
    {
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = "ppp.exe",
                                         WorkDir = null,
                                         Termination = new TerminationElement
                                                         {
                                                           Timeout = "42"
                                                         }
                                       }
                       });
    }

    [Test]
    public void Test_Execute_Test_Exe_Timeout_Invalid()
    {
      DoRawFailedSettingsTest(
        new ServiceSettings
          {
            Name = "jetService-test",
            DisplayName = "Jet Service Test",
            Description = "This is a jet service",
            Execution = new ExecutionElement
                          {
                            Arguments = null,
                            Program = "qqq.exe",
                            WorkDir = null,
                            Termination = new TerminationElement
                                            {
                                              Timeout = "zzz"
                                            }
                          }
          });
    }

    [Test]
    public void Test_Execute_Test_StopExe()
    {
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = "qqq.exe",
                                         WorkDir = null,
                                         Termination = new TerminationElement
                                                         {
                                                           Execution = new ExecutionBase
                                                                         {
                                                                           Arguments = null,
                                                                           Program = "ppp.exe",
                                                                           WorkDir = null,
                                                                         }
                                                         }
                                       }
                       });
    }

    [Test]
    public void Test_Execute_Test_StopExe_Dir()
    {
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = "www.exe",
                                         WorkDir = "aaa",
                                         Termination = new TerminationElement
                                                         {
                                                           Execution = new ExecutionBase
                                                                         {
                                                                           Arguments = null,
                                                                           Program = "ppp.exe",
                                                                           WorkDir = null,
                                                                         }
                                                         }
                                       }
                       },
                       (dir, e) => Directory.CreateDirectory(Path.Combine(dir, "aaa")));
    }

    [Test]
    public void Test_Execute_Test_StopExe_Dir2()
    {
      DoValidateSettingsTest(new ServiceSettings
                       {
                         Name = "jetService-test",
                         DisplayName = "Jet Service Test",
                         Description = "This is a jet service",
                         Execution = new ExecutionElement
                                       {
                                         Arguments = null,
                                         Program = "rewer",
                                         WorkDir = "aaa",
                                         Termination = new TerminationElement
                                                         {
                                                           Execution = new ExecutionBase
                                                                         {
                                                                           Arguments = "aasds",
                                                                           Program = "asds",
                                                                           WorkDir = "fff",
                                                                         }
                                                         }
                                       }
                       },
                       (dir, e) =>
                         {
                           Directory.CreateDirectory(Path.Combine(dir, "aaa"));
                           Directory.CreateDirectory(Path.Combine(dir, "fff"));
                         });
    }

    protected override void AssertServiceParameters(ServiceSettings s, JResult r, ExecutionElement ee)
    {
      var stopTimeout = 60*1000*ee.Termination.TerminateTimoeut;                                
      var lines = new List<string>
                    {
                      "[j]: ServiceName:" + s.Name + "!~",
                      "[j]: ServiceDisplayName:" + (s.DisplayName ?? s.Name) + "!~",
                      "[j]: ServiceDescription:" + s.Description + "!~",
                      @"[e]: Program:" + ee.Program + "!~",
                      @"[e]: Arguments:" + (ee.Arguments ?? "") + "!~",
                      @"[e]: WorkDir:" + ee.WorkDir + "!~",
                      @"[e]: StopTimeout:" + stopTimeout + "!~",
                    };
      if (ee.Termination != null)
      {
        var se = ee.Termination.Execution;
        if (se == null)
        {
          lines.Add("[s]: DISABLED:true!~");
        } else
        {
          lines.Add(@"[s]: DISABLED:false!~");
          lines.Add(@"[s]: Program:" + se.Program + "!~");
          lines.Add(@"[s]: Arguments:" + (se.Arguments ?? "") + "!~");
          lines.Add(@"[s]: WorkDir:" + se.WorkDir + "!~");
        }
      }

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
