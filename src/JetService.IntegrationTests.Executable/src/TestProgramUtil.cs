using System;
using System.Threading;

namespace JetService.IntegrationTests.Executable
{
  public class TestProgramUtil
  {
    public const string THREAD_NOT_FINISHED = "ERROR:THREAD:ERROR";

    protected static void RunInThreadAndWait(Action action)
    {
      RunInThreadAndWait(
        () =>
          {
            action();
            return "foo";
          });
    }

    protected static T RunInThreadAndWait<T>(Func<T> action)
    {
      var t = default(T);
      var th = new Thread(() => t = action());
      th.IsBackground = true;
      th.Start();
      if (!th.Join(TimeSpan.FromMinutes(1)))
      {
        Console.Out.WriteLine(THREAD_NOT_FINISHED);
        Console.Error.WriteLine(THREAD_NOT_FINISHED);
      }
      th.Interrupt();
      return t;
    }
  }
}