using System;

namespace JetService.IntegrationTests
{
  public class JetServiceCommandRunner
  {
    public static ProcessExecutor.Result ExecuteCommand(string command, params string[] arguments)
    {
      return TempFilesHolder.WithTempFile(
        tempFile =>
          {
            var r = ProcessExecutor.ExecuteProcess(Files.JetServiceExe, command + " /debug " + String.Join(" ", arguments));
            Console.Out.WriteLine("r = {0}", r);
            return r;
          });
    }
  }
}