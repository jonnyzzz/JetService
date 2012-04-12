using System;
using System.IO;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public static class JetServiceCommandRunner
  {
    public static JResult ExecuteCommand(string dir, string command, params string[] arguments)
    {
      UserManagement.GiveAllPermissions(dir);
      string tempFile = Path.Combine(dir, "serviceLogs.txt");
      var r = ProcessExecutor.ExecuteProcess(Files.JetServiceExe,
                                             command + " /logfile=" + tempFile + " /debug " +
                                             String.Join(" ", arguments));
      return new JResult
               {
                 BaseResult = r,
                 LogText = File.ReadAllText(tempFile),
                 LogFilePath = tempFile,
               };

    }
  }

  public class JResult
  {
    public ProcessExecutor.Result BaseResult { get; set; }
    public string LogText { get; set; }
    public int ExitCode { get { return BaseResult.ExitCode; } }
    public string LogFilePath { get; set; }
    public void AssertSuccess()
    {
      AssertNoErrorOutput().AssertExitedSuccessfully();
    }

    public JResult AssertOutputContains(params string[] text)
    {
      foreach (var _ in text)
      {
        var s = _.Trim();
        Assert.IsTrue(BaseResult.Output.Contains(s), "Process Output must contain {0}. Output: {1}", s, BaseResult.Output);
      }
      return this;
    }

    public JResult AssertNoErrorOutput()
    {
      Assert.IsTrue(string.IsNullOrWhiteSpace(BaseResult.Error));
      return this;
    }

    public JResult AssertExitedSuccessfully()
    {
      Assert.That(ExitCode, Is.EqualTo(0));
      return this;
    }

  }
}