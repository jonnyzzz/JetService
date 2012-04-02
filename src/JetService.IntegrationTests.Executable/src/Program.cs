using System;
using System.Linq;

namespace JetService.IntegrationTests.Executable
{
  public class TestProgram
  {
    public const string TEST_STDOUT = "test-stdout";
    public const string TEST_STDERR = "test-stderr";

    static int Main(string[] args)
    {

      switch (args.FirstOrDefault() ?? "")
      {
        case TEST_STDOUT:
          Console.Out.WriteLine("This is service std-out 44");
          return 0;

        case TEST_STDERR:
          Console.Error.WriteLine("This is service std-out 42");
          return 0;
        default:
          Console.Out.WriteLine("Unknwon");
          Console.Error.WriteLine("Unknwon");
          return 42;
      }
    }
  }
}
