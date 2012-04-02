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
      var r = ExecuteWithSettings(TestProgram.TEST_STDOUT);

      Console.Out.WriteLine(r.LogText);
      r.AssertSuccess();
    }

    [Test]
    public void std_err_captured()
    {
      var r = ExecuteWithSettings(TestProgram.TEST_STDERR);

      Console.Out.WriteLine(r.LogText);
      r.AssertSuccess();
    }



    private static JetServiceCommandRunner.Result ExecuteWithSettings(string arguemnts)
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
                                        Arguments = arguemnts,
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