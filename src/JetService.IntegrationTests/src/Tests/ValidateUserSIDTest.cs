using System;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class ValidateUserSIDTest : SettingsXmlTestBase
  {

    [Test]
    public void TestUserSID()
    {
      Console.Out.WriteLine("Identity: {0}",UserManagement.SID);
      UserManagement.WithNewUser(UserGroup.Admin,
        u => DoSettingsTest(DefaultSettins, "/user=" + u.UserName, "/password=" + u.Password, "/giveUserRights=false", "/checkUserAccount=false"));
    }


    protected override string ValidateCommandName
    {
      get { return "validate-sid"; }
    }

    protected override void AssertServiceParameters(ServiceSettings s, JResult r, ExecutionElement ee)
    {      
    }

    public ServiceSettings DefaultSettins
    {
      get
      {
        return new ServiceSettings
                 {
                   Name = "jetService-test",
                   DisplayName = "Jet Service Test",
                   Description = "This is a jet service",
                   Execution = new ExecutionElement
                                 {
                                   Arguments = null,
                                   Program = "Prog.exe",
                                   WorkDir = null
                                 }
                 };
      }
    }

    
  }
}