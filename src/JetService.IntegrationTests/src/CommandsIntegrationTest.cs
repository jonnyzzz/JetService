using NUnit.Framework;

namespace JetService.IntegrationTests
{
  [TestFixture]
  public class CommandsIntegrationTest
  {
    [Test]
    public void TestListCommands()
    {
      var r = JetServiceCommandRunner.ExecuteCommand("help");
      Assert.That(r.ExitCode, Is.EqualTo(0));
    }
  }
}
