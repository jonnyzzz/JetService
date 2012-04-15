#include "StdAfx.h"
#include "ProcessInterruptConsoleControlHandler.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptConsoleControlHandler");


ProcessInterruptConsoleControlHandler::ProcessInterruptConsoleControlHandler(const ServiceTaskSettings* settings)
  : myCtrlEventFailed(false)
  , myTerminateSent(0)
  , mySettings(settings)
{
}


ProcessInterruptConsoleControlHandler::~ProcessInterruptConsoleControlHandler(void)
{
}

void ProcessInterruptConsoleControlHandler::InterruptProcess(PROCESS_INFORMATION& info) {
  if (myCtrlEventFailed) {
    ProcessInterruptTerminateHandler::InterruptProcess(info);
    return;
  }

  if (myTerminateSent == 0) {
    LOG.LogInfo(L"Sending CRTL+C to the service process");
    if (0 == GenerateConsoleCtrlEvent(CTRL_C_EVENT, info.dwProcessId)) {
      myCtrlEventFailed = true;
      LOG.LogWarnFormat(L"Failed to stop process with CTRL+C. %s", LOG.GetLastError());
      return;
    }

    myTerminateSent = GetTickCount();
    return;
  } 

  ULONGLONG nowTicks = GetTickCount();
  if (nowTicks < myTerminateSent) {
    //workaround for 49.7 days running windows
    //http://msdn.microsoft.com/en-us/library/windows/desktop/ms724408(v=vs.85).aspx
    myTerminateSent = nowTicks;
  } else if (myTerminateSent < nowTicks + mySettings->getTerminateWaitTimeoutMilliseconds()) {
    //timeout has passed
    LOG.LogWarn(L"Failed to wait for process to exit by CTRL+C.");
    myCtrlEventFailed = true;
    return;
  }
}

