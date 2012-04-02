using NUnit.Framework;

namespace JetService.IntegrationTests
{
  [TestFixture]
  public class CommandsIntegrationTest
  {
    [Test]
    public void TestListCommands()
    {
      JetServiceCommandRunner.ExecuteCommand("help");
    }
  }
}