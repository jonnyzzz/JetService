using System;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  [TestFixture]
  public class RunUnderSystemTest : RunUnderServiceTestBase
  {
    protected override void ExecuteTestImpl(Action<string[]> runAction)
    {
      runAction(Stubs.A("/runAsSystem"));
    }
  }
}