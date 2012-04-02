using System;
using System.IO;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class ExecuteWithoutServiceTest
  {

    [Test]
    public void std_out_captured()
    {
      var r = ExecuteWithSettings(TestAction.TEST_STDOUT);

      Console.Out.WriteLine(r.LogText);
      Assert.That(r.LogText.Contains("This is service std-out 44"));
      r.AssertSuccess();
    }

    [Test]
    public void std_err_captured()
    {
      var r = ExecuteWithSettings(TestAction.TEST_STDERR);

      Console.Out.WriteLine(r.LogText);
      Assert.That(r.LogText.Contains("This is service std-err 42"));
      r.AssertSuccess();
    }

    [Test]
    public void std_in_closed()
    {
      var r = ExecuteWithSettings(TestAction.TEST_STDIN_READ);
      Console.Out.WriteLine(r.LogText);
      Assert.IsFalse(r.LogText.Contains(TestProgramUtil.THREAD_NOT_FINISHED));
      Assert.IsTrue(r.LogText.Contains("Try to read from console:Completed"));
      r.AssertSuccess();
    }

    private static JetServiceCommandRunner.Result ExecuteWithSettings(TestAction action, params string[] arguemnts)
    {
      return TempFilesHolder.WithTempDirectory(
        dir =>
          {

            var s = new ServiceSettings
                      {
                        Name = "some-test-service",
                        DisplayName = "some-test name",
                        Description = "some-descr",
                        Execution = new ServiceSettings.ExecutionElement
                                      {
                                        Program = Files.TestProgram,
                                        Arguments = action.ToString() + string.Join(" ", arguemnts),
                                        WorkDir = dir
                                      }
                      };

            var settings = Path.Combine(dir, "settings.xml");
            s.Serialize(settings);
            Console.Out.WriteLine("Settings: {0}", s);

            var r = JetServiceCommandRunner.ExecuteCommand("console", "/settings=" + settings);
            Console.Out.WriteLine(r.LogText);
            return r;
          }
        );
    }

  }
}