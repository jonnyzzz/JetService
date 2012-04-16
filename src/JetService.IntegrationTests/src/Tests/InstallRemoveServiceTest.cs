using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class InstallRemoveServiceTest : InstallRemoveServiceBase
  {
    [Test]
    public void InstallRemoveServiceTest_AsSystem()
    {
      InstallRemoveService(Stubs.A("/runAsSystem"), TestAction.Action(ConsoleTestAction.TEST_STDOUT));
    }

    [Test]
    public void InstallRemoveServiceTest_AsUserAdmin()
    {
      UserManagement.WithNewUser(UserGroup.Admin,
        u => InstallRemoveService(Stubs.A("/user=" + u.UserName, "/password=" + u.Password, "/checkUserAccount=false", "/giveUserRights=false"),
                                  TestAction.Action(ConsoleTestAction.TEST_STDOUT)));
    }
  }
}
