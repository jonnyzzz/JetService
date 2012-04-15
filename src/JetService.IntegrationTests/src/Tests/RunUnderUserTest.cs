using System;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class RunUnderUserTestBase : RunUnderServiceTestBase
  {
    protected abstract UserGroup UserGroup { get; }

    protected override void ExecuteTestImpl(Action<string[]> runAction)
    {
      UserManagement.WithNewUser(UserGroup, u => runAction(Stubs.A("/user=" + u.UserName, "/password=" + u.Password)));

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