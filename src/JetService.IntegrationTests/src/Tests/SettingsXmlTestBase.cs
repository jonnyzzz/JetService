using System;
using System.IO;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class SettingsXmlTestBase
  {
    
    protected abstract string ValidateCommandName { get; }
    protected abstract void AssertServiceParameters(ServiceSettings s,
                                                    JResult r,
                                                    ExecutionElement ee);

    protected JResult DoRawFailedSettingsTest(ServiceSettings s, Action<ServiceSettings, string> layout = null, params string[] cmd)
    {
      var r = DoRawSettingsTest(s, layout, cmd);
      Assert.That(r.ExitCode, Is.Not.EqualTo(0));
      return r;
    }

    protected delegate void OnAssertSettingsValidateRun(
      JResult result, 
      ServiceSettings settings, 
      ExecutionElement resolvedExec,
      string dir);

    protected JResult DoSettingsTest(ServiceSettings s, params string[] cmd)
    {
      return DoSettingsTest(
        s,
        (r, ss, ee, dir) =>
          {
            Assert.That(r.ExitCode, Is.EqualTo(0));

            AssertServiceParameters(s, r, ee);
          },
        cmd);
    }

    protected JResult DoSettingsTest(ServiceSettings s, OnAssertSettingsValidateRun postAction, string[] cmd)
    {

      Func<string, ExecutionElement, ExecutionElement> resolve =
        (dir, e) => new ExecutionElement
                      {
                        Arguments = e.Arguments,
                        Program = Path.Combine(dir, e.Program),
                        WorkDir = e.WorkDir == null ? dir : Path.Combine(dir, e.WorkDir),
                      };


      return TempFilesHolder.WithTempDirectory(
        dir =>
          {
            var ee = resolve(dir, s.Execution);
            if (!File.Exists(ee.Program))
            {
              File.WriteAllText(ee.Program, "mock");
            }


            var r = ExecuteWithSettings(s, dir, cmd);
            postAction(r, s, ee, dir);
            return r;
          });
    }


    private JResult DoRawSettingsTest(ServiceSettings s, Action<ServiceSettings, string> layout, string[] cmd)
    {
      return TempFilesHolder.WithTempDirectory(
        dir =>
          {
            if (layout != null) layout(s, dir);
            return ExecuteWithSettings(s, dir, cmd);
          });
    }

    private JResult ExecuteWithSettings(ServiceSettings s, string dir, string[] cmd)
    {
      var settings = Path.Combine(dir, "settings.xml");
      s.Serialize(settings);
      Console.Out.WriteLine("Settings: {0}", s);

      var r = JetServiceCommandRunner.ExecuteCommand(dir, ValidateCommandName, "/settings=" + settings + " " + string.Join(" ", cmd));
      Console.Out.WriteLine(r.LogText);
      return r;
    }
  }
}