using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class RunUnderSystemTest : RunUnderServiceTestBase
  {
    protected override void ExecuteTestImpl(TestAction testAction, GenerateServiceExecutableArguments argz, OnServiceInstalled onInstalled)
    {
      InstallRemoveService(Stubs.A("/runAsSystem"),
                     testAction,
                     argz,
                     onInstalled);

    }
  }
}