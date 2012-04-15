#include "StdAfx.h"
#include "ProcessInterruptTimeoutHandler.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptTimeoutHandler");

ProcessInterruptTimeoutHandler::ProcessInterruptTimeoutHandler(const ServiceTaskSettings* settings)
  : myActionFailed(false)
  , myTerminateSent(0)
  , mySettings(settings)  
{
}


ProcessInterruptTimeoutHandler::~ProcessInterruptTimeoutHandler(void)
{
}


void ProcessInterruptTimeoutHandler::OnTimeoutFailed(PROCESS_INFORMATION& info) {
  myTerminate.InterruptProcess(info);
}

void ProcessInterruptTimeoutHandler::InterruptProcess(PROCESS_INFORMATION& info) {
  if (myActionFailed) {
    OnTimeoutFailed(info);
    return;
  }

  if (myTerminateSent == 0) {
    myActionFailed = !ExecuteInterruptAction(info);
    if (!myActionFailed) myTerminateSent = GetTickCount();
    return;
  } 

  ULONGLONG nowTicks = GetTickCount();
  if (nowTicks < myTerminateSent) {
    //workaround for 49.7 days running windows
    //http://msdn.microsoft.com/en-us/library/windows/desktop/ms724408(v=vs.85).aspx
    myTerminateSent = nowTicks;
    return;
  } 
  
  if (myTerminateSent + mySettings->getTerminateWaitTimeoutMilliseconds() < nowTicks) {
    //timeout
    LOG.LogWarn(L"Failed to wait for process to exit...");
    myActionFailed = true;    
    return;
  } 

  LOG.LogDebugFormat(L"Waiting for process to exit...");
}

