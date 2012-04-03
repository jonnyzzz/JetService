using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class RunUnderSystemTest : RunUnderServiceTestBase
  {
    protected override void ExecuteTestImpl(TestAction testAction, string[] argz, OnServiceInstalled onInstalled)
    {
      InstallRemoveService(A("/runAsSystem"),
                     testAction,
                     argz,
                     onInstalled);

    }
  }
}