using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class CommandsIntegrationTest
  {
    [Test]
    public void TestListCommands()
    {
      TempFilesHolder.WithTempDirectory(
        dir =>
          {
            var r = JetServiceCommandRunner.ExecuteCommand(dir, "help");
            Assert.That(r.ExitCode, Is.EqualTo(0));
          });
    }
  }
}
