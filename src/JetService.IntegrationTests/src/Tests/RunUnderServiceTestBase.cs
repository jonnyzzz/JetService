using System;
using System.IO;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class RunUnderServiceTestBase : InstallRemoveServiceBase
  {
    protected abstract void ExecuteTestImpl(TestAction testAction, GenerateServiceExecutableArguments argz, OnServiceInstalled onInstalled);

    private void DoExecuteTest(TestAction testAction, string[] argz, Action<ServiceSettings, string> onInstalled)
    {
      DoExecuteTest(testAction, (a, b) => argz, onInstalled);
    }

    private void DoExecuteTest(TestAction testAction, GenerateServiceExecutableArguments argz, Action<ServiceSettings, string> onInstalled)
    {
      ExecuteTestImpl(testAction, argz, (s, dir, log) => StartStopService(s, log, () =>  onInstalled(s, dir), true));
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


    [Test]
    public void TestStartStopServiceFromService()
    {
      TempFilesHolder.WithTempFile(
        file =>
        {
          File.Delete(file);
          DoExecuteTest(TestAction.TEST_STOP_SERVICE, (action, name) => Stubs.A(file, name), 
            (s,dir) =>
              {
                WaitFor.WaitForAssert(TimeSpan.FromSeconds(30), TimeSpan.FromSeconds(.5),
                  () => !ServicesUtil.IsServiceNotStopped(s),
                  "Service must be able to stop itself");
                ;
              });

          Assert.IsTrue(File.Exists(file));
        });
    }

    [Test]
    public void TestSTDInProcessShouldNotHungInService()
    {
      DoExecuteTest(TestAction.TEST_SERVICE_STDIN_READ, Stubs.A(),
                    (s, dir) =>
                      {
                        WaitFor.WaitForAssert(TimeSpan.FromSeconds(30), TimeSpan.FromSeconds(.5),
                                              () => !ServicesUtil.IsServiceNotStopped(s),
                                              "Service must exit itself");
                        ;
                      });
    }

    [Test, Ignore("Failed to sent signal")]
    public void Test_CTRL_C_on_service_stop()
    {
      TempFilesHolder.WithTempFile(
        file =>
        {
          File.Delete(file);
          DoExecuteTest(TestAction.TEST_STOP_EVENT_HANDLED, Stubs.A(file, "false"), 
            (s, dir) =>
              {
              }
            );

          Assert.IsTrue(File.Exists(file));
        });
    }
  }
}