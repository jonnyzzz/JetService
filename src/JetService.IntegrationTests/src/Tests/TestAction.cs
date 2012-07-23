using System;
using JetService.IntegrationTests.Executable;
using JetService.IntegrationTests.WinForms;

namespace JetService.IntegrationTests.Tests
{
  public class TestAction
  {
    private TestAction() { }

    private string ActionName { get; set; }
    private Func<string, string[]> Arguments { get; set; }
    private string Program { get; set; }

    public void InitializeProgramArguments(ExecutionBase el, string dir, string serviceName)
    {
      el.Arguments = ActionName + " " + string.Join(" ", Arguments(serviceName));
      el.Program = Program;
      el.WorkDir = dir;
    }

    public static TestAction Action(ConsoleTestAction a, params string[] argz)
    {
      return Action(a, _ => argz);
    }

    public static TestAction Action(WinFormsTestAction a, params string[] argz)
    {
      return Action(a, _ => argz);
    }

    public static TestAction Action(ConsoleTestAction a, GenerateServiceExecutableArguments az = null)
    {
      Func<string, string[]> nop = x => new string[0];
      return new TestAction
               {
                 Program = Files.ConsoleTestProgram,
                 ActionName = a.ToString(),
                 Arguments = az == null ? nop : x => az(x)
               };
    }

    public static TestAction Action(WinFormsTestAction a, GenerateServiceExecutableArguments az = null)
    {
      Func<string, string[]> nop = x => new string[0];
      return new TestAction
               {
                 Program = Files.WinFormsTestProgram,
                 ActionName = a.ToString(),
                 Arguments = az == null ? nop : x => az(x)
               };
    }

  }
}