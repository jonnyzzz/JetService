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
      InstallRemoveService(A("/runAsSystem"),
                           TestAction.TEST_STDOUT,
                           A(),
                           (s, dir, log) => { }
        );
    }

    [Test]
    public void InstallRemoveServiceTest_AsUserAdmin()
    {
      UserManagement.WithNewUser(UserGroup.Admin,
        u => InstallRemoveService(A("/user=" + u.UserName, "/password=" + u.Password, "/checkUserAccount=false"),
                                  TestAction.TEST_STDOUT,
                                  A(),
                                  (s, dir, log) => { }
               ));
    }
  }
}