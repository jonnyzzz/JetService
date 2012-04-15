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

    protected delegate void OnResolveSettings(string dir, ExecutionElement e);

    protected JResult DoValidateSettingsTest(ServiceSettings s, params string[] cmd)
    {
      return DoValidateSettingsTest(s, Stubs.NOP, cmd);
    }

    protected JResult DoValidateSettingsTest(ServiceSettings s, OnResolveSettings resolve, params string[] cmd)
    {
      return DoSettingsTest(
        s,
        resolve,
        (r, ss, ee, dir) =>
          {
            Assert.That(r.ExitCode, Is.EqualTo(0));

            AssertServiceParameters(s, r, ee);
          },
        cmd);
    }

    protected JResult DoSettingsTest(ServiceSettings s, OnResolveSettings resolveSettings, OnAssertSettingsValidateRun postAction, string[] cmd)
    {

      Func<string, ExecutionElement, ExecutionElement> resolve =
        (dir, e) =>
          {
            var ret = e.SClone();
            ret.Program = Path.Combine(dir, e.Program);
            ret.WorkDir = e.WorkDir == null ? dir : Path.Combine(dir, e.WorkDir);

            if (ret.Termination != null)
            {
              if (ret.Termination.Timeout == null) ret.Termination.Timeout = "2";
              if (ret.Termination.Execution != null)
              {
                var se = ret.Termination.Execution;
                se.Program = Path.Combine(dir, se.Program);
                se.WorkDir = se.WorkDir == null ? ret.WorkDir : Path.Combine(dir, se.WorkDir);
              }
            } else
            {
              ret.Termination = new TerminationElement
                                  {
                                    Timeout = "2"
                                  };
            }

            resolveSettings(dir, ret);
            return ret;
          };

      return TempFilesHolder.WithTempDirectory(
        dir =>
          {
            var ee = resolve(dir, s.Execution);
            if (!File.Exists(ee.Program))
            {
              File.WriteAllText(ee.Program, "mock");
            }

            if (ee.Termination != null && ee.Termination.Execution != null && !File.Exists(ee.Termination.Execution.Program))
            {
              File.WriteAllText(ee.Termination.Execution.Program, "smock");
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