using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
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
