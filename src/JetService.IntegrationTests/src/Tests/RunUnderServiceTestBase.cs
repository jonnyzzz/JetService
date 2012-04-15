using System;
using System.IO;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class RunUnderServiceTestBase : InstallRemoveServiceBase
  {
    protected abstract void ExecuteTestImpl(Action<string[]> runAction);

    protected void ExecuteTestImpl(TestAction testAction, GenerateServiceExecutableArguments argz,
      TestAction? stopAction, GenerateServiceExecutableArguments stopArgz,       
      OnServiceInstalled onInstalled)
    {
      ExecuteTestImpl(az => InstallRemoveService(az,
                                                 testAction,
                                                 argz,
                                                 stopAction,
                                                 stopArgz,
                                                 onInstalled));
    }

    private void DoExecuteTest(TestAction testAction, GenerateServiceExecutableArguments argz, 
                               TestAction? stopAction, GenerateServiceExecutableArguments stopArgz, 
                               Action<ServiceSettings, string> onInstalled)
    {
      OnServiceInstalled onServiceInstalled = (s, dir, log) => StartStopService(s, log, () => onInstalled(s, dir), true);

      ExecuteTestImpl(testAction, argz, stopAction, stopArgz, onServiceInstalled);
    }

    private void DoExecuteTest(TestAction testAction, GenerateServiceExecutableArguments argz, Action<ServiceSettings, string> onInstalled)
    {
      DoExecuteTest(testAction, argz, null, null, onInstalled);
    }

    [Test]
    public void TestProcessStartStop()
    {
       DoExecuteTest(TestAction.TEST_RUN_10500, (a,b) => Stubs.A(), Stubs.NOP);
    }

    [Test]
    public void TestProcessRunningUnderService()
    {
      TempFilesHolder.WithTempFile(
        file =>
          {
            File.Delete(file);
            DoExecuteTest(TestAction.TEST_IM_ALIVE, (a,b) => Stubs.A(file), Stubs.NOP);
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
                WaitFor.WaitForAssert(TimeSpan.FromSeconds(60), TimeSpan.FromSeconds(.5),
                  () => ServicesUtil.IsServiceStopped(s),
                  "Service must be able to stop itself");
                ;
              });

          Assert.IsTrue(File.Exists(file));
        });
    }

    [Test]
    public void TestStartStopServiceFromService_action()
    {
      TempFilesHolder.WithTempFile(
        file_start =>
        TempFilesHolder.WithTempFile(
          file_stop =>
            {
              File.Delete(file_start);
              DoExecuteTest(
                TestAction.TEST_IM_ALIVE, (action, name) => Stubs.A(file_start, name),
                TestAction.TEST_WRITE_FILE, (action, name) => Stubs.A(file_stop, name),
                            (s, dir) =>
                              {                             
                                ;
                              });

              Assert.IsTrue(File.Exists(file_start));
              Assert.IsTrue(File.Exists(file_stop));
            }));
    }

    [Test]
    public void TestSTDInProcessShouldNotHungInService()
    {
      DoExecuteTest(TestAction.TEST_SERVICE_STDIN_READ, (A,b) => Stubs.A(),
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
          DoExecuteTest(TestAction.TEST_STOP_EVENT_HANDLED, (a,b) => Stubs.A(file, "false"), 
            (s, dir) =>
              {
              }
            );

          Assert.IsTrue(File.Exists(file));
        });
    }
  }
}