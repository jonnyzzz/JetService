using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class RunUnderAdminUserTest : RunUnderServiceTestBase
  {
    protected override void ExecuteTestImpl(TestAction testAction, string[] argz, OnServiceInstalled onInstalled)
    {
      UserManagement.WithNewUser(UserGroup.Admin,
                                 u => InstallRemoveService(
                                   Stubs.A("/user=" + u.UserName, "/password=" + u.Password),
                                   testAction,
                                   argz,
                                   onInstalled));
    }
  }

  [TestFixture]
  public class RunUnderUserUserTest : RunUnderServiceTestBase
  {
    protected override void ExecuteTestImpl(TestAction testAction, string[] argz, OnServiceInstalled onInstalled)
    {
      UserManagement.WithNewUser(UserGroup.User,
                                 u => InstallRemoveService(
                                   Stubs.A("/user=" + u.UserName, "/password=" + u.Password),
                                   testAction,
                                   argz,
                                   onInstalled));
    }
  }
}