using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class RunUnderUserTestBase : RunUnderServiceTestBase
  {
    protected abstract UserGroup UserGroup { get; }

    protected sealed override void ExecuteTestImpl(TestAction testAction, GenerateServiceExecutableArguments argz, OnServiceInstalled onInstalled)
    {
      UserManagement.WithNewUser(UserGroup,
                                 u => InstallRemoveService(
                                   Stubs.A("/user=" + u.UserName, "/password=" + u.Password),
                                   testAction,
                                   argz,
                                   onInstalled));
    }

  }

  [TestFixture]
  public class RunUnderAdminUserTest : RunUnderUserTestBase
  {
    protected override UserGroup UserGroup
    {
      get { return UserGroup.Admin; }
    }
  }

  [TestFixture]
  public class RunUnderUserUserTest : RunUnderUserTestBase
  {
    protected override UserGroup UserGroup
    {
      get { return UserGroup.User; }
    }
  }
}