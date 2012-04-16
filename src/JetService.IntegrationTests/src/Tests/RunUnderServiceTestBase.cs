using System;
using System.IO;
using JetService.IntegrationTests.Executable;
using JetService.IntegrationTests.WinForms;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public abstract class RunUnderServiceTestBase : InstallRemoveServiceBase
  {
    protected abstract void ExecuteTestImpl(Action<string[]> runAction);

    private void DoExecuteTest(TestAction startAction, Action<ServiceSettings, string> onInstalled = null, TestAction stopAction = null)
    {
      Action nop = () => { };
      OnServiceInstalled onServiceInstalled =
        (s, dir, log) =>
          {
            Action afterStarted = onInstalled == null ? nop : () => onInstalled(s, dir);
            StartStopService(s, log, afterStarted, true);
          };

      ExecuteTestImpl(az => InstallRemoveService(az, startAction, onServiceInstalled, stopAction));
    }

    [Test]
    public void TestProcessStartStop()
    {
       DoExecuteTest(TestAction.Action(ConsoleTestAction.TEST_RUN_10500));
    }

    [Test]
    public void TestProcessStartStop_GUI()
    {
       DoExecuteTest(TestAction.Action(WinFormsTestAction.TEST_OPEN));
    }

    [Test]
    public void TestProcessRunningUnderService()
    {
      TempFilesHolder.WithTempFile(
        file =>
          {
            File.Delete(file);
            DoExecuteTest(TestAction.Action(ConsoleTestAction.TEST_IM_ALIVE, file));
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
          DoExecuteTest(TestAction.Action(ConsoleTestAction.TEST_STOP_SERVICE, name => Stubs.A(file, name)),
                        (s, dir) => WaitFor.WaitForAssert(
                          TimeSpan.FromSeconds(60),
                          () => TimeSpan.FromSeconds(.5),
                          () => ServicesUtil.IsServiceStopped(s),
                          () => "Service must be able to stop itself. Status: " + ServicesUtil.DumpServices(s)));

          Assert.IsTrue(File.Exists(file));
        });
    }

    [Test]
    public void TestStartStopServiceSTDIN()
    {
      TempFilesHolder.WithTempFile(
        file =>
        {
          File.Delete(file);
          DoExecuteTest(TestAction.Action(ConsoleTestAction.TEST_STDIN_SLEEP_READ, name => Stubs.A(file, name)),
                        (s, dir) => WaitFor.WaitForAssert(
                          TimeSpan.FromSeconds(60),
                          () => TimeSpan.FromSeconds(.5),
                          () => ServicesUtil.IsServiceStopped(s),
                          () => "Service must not hung reading STDIN. " + ServicesUtil.DumpServices(s)));
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
                TestAction.Action(ConsoleTestAction.TEST_IM_ALIVE, name => Stubs.A(file_start, name)),
                Stubs.NOP,
                TestAction.Action(ConsoleTestAction.TEST_WRITE_FILE, name => Stubs.A(file_stop, name))
                );

              Assert.IsTrue(File.Exists(file_start));
              Assert.IsTrue(File.Exists(file_stop));
            }));
    }

    [Test]
    public void TestSTDInProcessShouldNotHungInService()
    {
      DoExecuteTest(TestAction.Action(ConsoleTestAction.TEST_SERVICE_STDIN_READ),
                    (s, dir) => WaitFor.WaitForAssert(
                      TimeSpan.FromSeconds(30),
                      () => TimeSpan.FromSeconds(.5),
                      () => !ServicesUtil.IsServiceNotStopped(s),
                      () => "Service must exit itself"));
    }


    [Test, Ignore("Failed to sent signal")]
    public void Test_CTRL_C_on_service_stop()
    {
      TempFilesHolder.WithTempFile(
        file =>
        {
          File.Delete(file);
          DoExecuteTest(TestAction.Action(ConsoleTestAction.TEST_STOP_EVENT_HANDLED, file, "false"));
          Assert.IsTrue(File.Exists(file));
        });
    }
  }
}