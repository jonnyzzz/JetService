#include "StdAfx.h"
#include "ProcessInterruptCallProcessHandler.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessCommand.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptCallProcessHandler");


ProcessInterruptCallProcessHandler::ProcessInterruptCallProcessHandler(const ServiceTaskSettings* settings)
  : ProcessInterruptTimeoutHandler(settings)
  , myThread(NULL)
  , myThreadId(0)
{
}


ProcessInterruptCallProcessHandler::~ProcessInterruptCallProcessHandler()
{
  myStopInterruptFlag.Interrupt();
  WaitForThreadToExit();
}

void ProcessInterruptCallProcessHandler::WaitForThreadToExit() {
  if (myThread == NULL) return;

  if (WaitForSingleObject(myThread, 500) == WAIT_TIMEOUT) {
    LOG.LogWarn(L"Failed to wait for stop service thread to exit");
  }
  WaitForSingleObject(myThread, INFINITE);
  myThread = NULL;
}

void ProcessInterruptCallProcessHandler::ThreadProcess() {
  const ExecutionSettings* stop = mySettings->getStopCommand();
  if (stop == NULL) return;

  LOG.LogDebug(L"Executing stop command...");
  ProcessCommand cmd(stop, &myStopInterruptAction, &myStopInterruptFlag);
  static_cast<Command*>(&cmd)->executeCommand();
}

DWORD WINAPI ThreadedServiceTaskThreadProc2(LPVOID lpParameter) {
  ((ProcessInterruptCallProcessHandler*)lpParameter)->ThreadProcess();
  return 0;
}

bool ProcessInterruptCallProcessHandler::ExecuteInterruptAction(PROCESS_INFORMATION& info) {
  const ExecutionSettings* stop = mySettings->getStopCommand();
  if (stop == NULL) return false;

  myThread = CreateThread(NULL, 0, &ThreadedServiceTaskThreadProc2, this, 0, &myThreadId);
  if (myThread == NULL) {
    LOG.LogError(L"Failed to create thread to execute service stop task");    
    myThread = NULL;
    myThreadId = 0;
    return false;
  }
  LOG.LogDebug(L"Started stop service task thread");
  return true;
}

void ProcessInterruptCallProcessHandler::OnTimeoutFailed(PROCESS_INFORMATION& info) {
  myStopInterruptFlag.Interrupt();
  WaitForThreadToExit();
  this->ProcessInterruptTimeoutHandler::OnTimeoutFailed(info);
}

