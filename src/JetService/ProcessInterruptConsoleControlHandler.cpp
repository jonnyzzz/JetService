#include "StdAfx.h"
#include "ProcessInterruptConsoleControlHandler.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptConsoleControlHandler");


ProcessInterruptConsoleControlHandler::ProcessInterruptConsoleControlHandler(void)
  : myCtrlEventFailed(false)
  , myTerminateSent(0)
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
    if (0 == GenerateConsoleCtrlEvent(CTRL_C_EVENT, info.dwProcessId)) {
      myCtrlEventFailed = true;
      LOG.LogWarnFormat(L"Failed to initiate terminate of process with Console Ctrl Event. Process will be killed. %s", LOG.GetLastError());
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
  } else if (myTerminateSent < nowTicks + 1000L * 60 * 15) {
    //timeout has passed
    LOG.LogWarnFormat(L"Failed to wait for process to exit by CTRL+C. Process will be terminated");
    myCtrlEventFailed = true;
    return;
  }
}

