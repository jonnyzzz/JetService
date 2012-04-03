using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class UserManagerTest
  {
    [TestCase]
    public void TestCanCreateDeleteUser_Admin()
    {
      DoUserAddRemoveTest(UserGroup.Admin);
    }

    [TestCase]
    public void TestCanCreateDeleteUser_Guest()
    {
      DoUserAddRemoveTest(UserGroup.Guest);
    }

    [TestCase]
    public void TestCanCreateDeleteUser_User()
    {
      DoUserAddRemoveTest(UserGroup.User);
    }

    [TestCase]
    public void TestCanCreateDeleteUser_PowerUser()
    {
      DoUserAddRemoveTest(UserGroup.PowerUser);
    }

    private static void DoUserAddRemoveTest(UserGroup userGroup)
    {
      User u = null;
      UserManagement.WithNewUser(
        userGroup,
        user =>
          {
            u = user;
            Assert.IsTrue(UserManagement.CheckUserExists(u));
          });
      if (u != null)
      {
        Assert.IsFalse(UserManagement.CheckUserExists(u));
      }
    }
  }
}