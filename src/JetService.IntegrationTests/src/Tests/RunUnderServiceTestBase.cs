using System;
using System.IO;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class RunUnderServiceTestBase : InstallRemoveServiceBase
  {
    protected abstract void ExecuteTestImpl(TestAction testAction, string[] argz, OnServiceInstalled onInstalled);

    private void DoExecuteTest(TestAction testAction, string[] argz, Action<ServiceSettings, string> onInstalled)
    {
      ExecuteTestImpl(testAction, argz, (s, dir, log) => StartStopService(s, log, () =>  onInstalled(s, dir)));
    }

    [Test]
    public void TestProcessStartStop()
    {
       DoExecuteTest(TestAction.TEST_RUN_10500, Stubs.A(), Stubs.NOP);
    }

    [Test]
    public void TestProcessRunningUnderService()
    {
      TempFilesHolder.WithTempFile(
        file =>
          {
            File.Delete(file);
            DoExecuteTest(TestAction.TEST_IM_ALIVE, Stubs.A(file), Stubs.NOP);
            Assert.IsTrue(File.Exists(file));
          });
    }
  }
}