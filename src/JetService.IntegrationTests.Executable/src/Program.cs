using System;
using System.Linq;

namespace JetService.IntegrationTests.Executable
{
  public enum TestAction
  {
    TEST_STDOUT, 
    TEST_STDERR,
    TEST_STDIN_READ,

    UNKNOWN
  }

  public class TestProgram : TestProgramUtil
  {
    static int Main(string[] args)
    {
      TestAction result;
      if (!Enum.TryParse(args.FirstOrDefault(), true, out result))
        result = TestAction.UNKNOWN;

      switch (result)
      {
        case TestAction.TEST_STDOUT:
          Console.Out.WriteLine("This is service std-out 44");
          return 0;

        case TestAction.TEST_STDERR:
          Console.Out.WriteLine("This is service std-err 42");
          return 0;

        case TestAction.TEST_STDIN_READ:
          Console.Error.WriteLine("Try to read from console");
          RunInThreadAndWait(() => Console.In.Read());
          Console.Error.WriteLine("Try to read from console:Completed");
          return 0;

        default:
        case TestAction.UNKNOWN:        
          Console.Out.WriteLine("Unknwon");
          Console.Error.WriteLine("Unknwon");
          return 42;
      }
    }
  }
}
