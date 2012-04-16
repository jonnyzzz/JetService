using System;
using System.IO;
using System.Threading;
using JetService.IntegrationTests.Executable;
using NUnit.Framework;

namespace JetService.IntegrationTests.Tests
{
  public class InstallRemoveServiceBase
  {
    protected void StartStopService(ServiceSettings service, string log, Action afterStarted, bool waitStarted = false)
    {
      var stopRead = false;
      var logsReader = new Thread(
        () =>
          {
            if (log == null) return;
            File.AppendAllText(log, "=====\r\n\r\n");
            try
            {
              Action<string> logLine = str => Console.Out.WriteLine("[service log] {0}", str);              
              using (var s = new FileStream(log, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
              using (var r = new StreamReader(s))
              {
                string str;
                while ((str = r.ReadLine()) != null)
                {
                  logLine(str);                  
                  Thread.Sleep(100);
                  if (stopRead) break;
                }
                foreach (var tmp in r.ReadToEnd().Split(new[] { "\r\n", "\r", "\n" }, StringSplitOptions.None))
                {
                  logLine(tmp);
                }
              }
            } catch(Exception e)
            {
              Console.Out.WriteLine("Failed to read service log: " + e);
            }
          });

      logsReader.Name = "Service logs reader";
      logsReader.IsBackground = true;
      using (var c = new Catcher())
      {
        c.Execute(
          logsReader.Start,
          () =>
            {
              stopRead = true;
              logsReader.Join();

            }
          );

        c.Execute(
          () => ServicesUtil.StartService(service).AssertExitedSuccessfully(),
          () =>
            {
              if (ServicesUtil.IsServiceNotStopped(service))
              {
                Console.Out.WriteLine("Stopping service...");
                ServicesUtil.StopService(service).AssertExitedSuccessfully();
              }
              else
              {
                Console.Out.WriteLine("Service is stopped. Nothing to stop");
              }
            }
          );
        if (waitStarted)
        {
          c.Execute(
            () =>
              {
                WaitFor.WaitForAssert(TimeSpan.FromSeconds(30), () => TimeSpan.FromSeconds(.5),
                                      () => ServicesUtil.IsServiceRunning(service),
                                      () => "Service must report as running. Status: " + ServicesUtil.DumpServices(service));
                ;
              });
        }
        c.Execute(afterStarted);
      }
    }

    protected delegate void OnServiceInstalled(ServiceSettings setting, string dir, string logFile);

    protected delegate string[] GenerateServiceExecutableArguments(TestAction action, string serviceName);

    protected void InstallRemoveService(string[] installServiceArguments,
                                        TestAction action,
                                        GenerateServiceExecutableArguments testProgramArguments,
                                        OnServiceInstalled afterInstalled)
    {
      InstallRemoveService(installServiceArguments, action, testProgramArguments, null, null, afterInstalled);
    }
    
    protected void InstallRemoveService(string[] installServiceArguments,
                                        TestAction action,
                                        GenerateServiceExecutableArguments testProgramArguments,
                                        TestAction? stopAction,
                                        GenerateServiceExecutableArguments stopProgramArguments,
                                        OnServiceInstalled afterInstalled)
    {
      ServicesUtil.AssertHasInstallServiceRights();

      TempFilesHolder.WithTempDirectory(
        dir =>
        {
          UserManagement.GiveAllPermissions(dir);
          var settingsXml = CreateSettingsXml(dir, action, testProgramArguments, stopAction, stopProgramArguments);
          var settings = Path.Combine(dir, "settings.xml");
          settingsXml.Serialize(settings);
          Console.Out.WriteLine("Settings: {0}", settingsXml);

          using (var c = new Catcher())
          {
            c.Execute(
              () => { }, 
              () =>EnsureServiceRemoved(settingsXml)
              );

            var logFile = c.Execute(
              () =>
                {
                  var r = JetServiceCommandRunner.ExecuteCommand(dir, "install",
                                                                 "/settings=" + settings + " " +
                                                                 string.Join(" ", installServiceArguments));
                  Console.Out.WriteLine(r.LogText);
                  r.AssertSuccess();
                  return r.LogFilePath;
                },
              () =>
                {
                  //wait for service to exit before stopping it
                  for (int i = 0; i < 100; i++)
                  {
                    if(ServicesUtil.IsServiceNotStopped(settingsXml)) break;
                    Thread.Sleep(100);
                  }

                  var r = JetServiceCommandRunner.ExecuteCommand(dir, "delete", "/settings=" + settings);
                  Console.Out.WriteLine(r.LogText);
                  r.AssertSuccess();
                }
              );

            c.Execute(() => EnsureServiceInstalled(settingsXml));
            c.Execute(() => afterInstalled(settingsXml, dir, logFile));
          }
        });
    }

    private static void InitializeProgramArtguments(ExecutionBase el, string dir, string serviceName, TestAction action, GenerateServiceExecutableArguments az)
    {
      el.Arguments = action + " " + string.Join(" ", az(action, serviceName));
      el.Program = Files.TestProgram;
      el.WorkDir = dir;
    }

    private static ServiceSettings CreateSettingsXml(string dir, 
      TestAction action, GenerateServiceExecutableArguments testProgramArguments, 
      TestAction? stopAction, GenerateServiceExecutableArguments stopProgramArguments,
      double? timeout = 0.1707)
    {
      var hash = (int) (DateTime.Now - new DateTime(2012, 04, 01)).TotalMilliseconds%9999;
      string serviceName = "jetService-test-" + hash;
      ExecutionBase stopActionEl = null;
      if (stopAction != null)
      {
        stopActionEl = new ExecutionBase();
        InitializeProgramArtguments(stopActionEl, dir, serviceName, stopAction.Value, stopProgramArguments);        
      }
      var settingsXml = new ServiceSettings
                          {
                            Name = serviceName,
                            Description = "This is a jet service " + hash,
                            Execution = new ExecutionElement
                                          {                                            
                                            Termination = new TerminationElement
                                                            {
                                                              Timeout = timeout == null ? null : timeout.ToString().Replace(",", "."),
                                                              Execution = stopActionEl
                                                            }
                                          }
                          };
      InitializeProgramArtguments(settingsXml.Execution, dir, serviceName, action, testProgramArguments);
      return settingsXml;
    }

    private static void EnsureServiceInstalled(ServiceSettings settingsXml)
    {
      Thread.Sleep(TimeSpan.FromSeconds(1));
      Assert.IsTrue(ServicesUtil.IsServiceInstalled(settingsXml), "Service must be installed: {0}", settingsXml.Name);
    }

    private static void EnsureServiceRemoved(ServiceSettings settingsXml)
    {
      WaitFor.WaitForAssert(
        () => !ServicesUtil.IsServiceInstalled(settingsXml),
        () => string.Format("Service must be uninstalled: {0}", settingsXml.Name)
        );
    }

    [TestFixtureTearDown]
    public void FixtureTearDown()
    {
      try
      {
        ServicesUtil.RemoveServices(x => x.ToLower().StartsWith("jetService-test".ToLower()));
      } catch(Exception e) 
      {
        Console.Out.WriteLine("Failed to cleanup services. {0}", e);
      }
    }
  }
}
