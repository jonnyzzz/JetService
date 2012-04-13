#include "StdAfx.h"
#include "ProcessInterruptTerminateHandler.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptTerminateHandler");

ProcessInterruptTerminateHandler::ProcessInterruptTerminateHandler(void)
{
}


ProcessInterruptTerminateHandler::~ProcessInterruptTerminateHandler(void)
{
}


void ProcessInterruptTerminateHandler::InterruptProcess(PROCESS_INFORMATION& info) {
  //block from second call
  if (IsInterrupted()) return;
  

  LOG.LogInfo(L"Service process will be terminated");
  //TODO:implement kill process
  if (0 == TerminateProcess(info.hProcess, 42)) {
    LOG.LogWarnFormat(L"Failed to terminate service process. %s", LOG.GetLastError());
    return;
  }

  //mark interrupt to the process was sent
  Interrupt();
}
