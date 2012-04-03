using System;
using System.IO;

namespace JetService.IntegrationTests
{
  public class JetServiceCommandRunner
  {
    public class Result
    {
      public ProcessExecutor.Result BaseResult { get; set; }
      public string LogText { get; set; }
      public int ExitCode { get { return BaseResult.ExitCode; } }
      public string LogFilePath { get; set; }
      public void AssertSuccess()
      {
        BaseResult.AssertNoErrorOutput().AssertExitedSuccessfully();
      }
    }

    public static Result ExecuteCommand(string command, params string[] arguments)
    {
      return TempFilesHolder.WithTempFile(
        tempFile =>
          {
            var r = ProcessExecutor.ExecuteProcess(Files.JetServiceExe, command + " /logfile=" + tempFile + " /debug " + String.Join(" ", arguments));
            return new Result
                     {
                       BaseResult = r,
                       LogText = File.ReadAllText(tempFile),
                       LogFilePath = tempFile,
                     };
          });
    }
  }
}