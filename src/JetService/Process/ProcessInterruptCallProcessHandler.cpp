#include "StdAfx.h"
#include "ProcessInterruptCallProcessHandler.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessCommand.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptCallProcessHandler");


ProcessInterruptCallProcessHandler::ProcessInterruptCallProcessHandler(const ServiceTaskSettings* settings)
  : ProcessInterruptTimeoutHandler(settings)
{
}


ProcessInterruptCallProcessHandler::~ProcessInterruptCallProcessHandler()
{
}


bool ProcessInterruptCallProcessHandler::ExecuteInterruptAction(PROCESS_INFORMATION& info) {
  const ExecutionSettings* stop = mySettings->getStopCommand();
  if (stop == NULL) return false;

  LOG.LogDebug(L"Executing stop command...");
  ProcessCommand cmd(stop, &myStopInterruptAction, &myStopInterruptFlag);
  static_cast<Command*>(&cmd)->executeCommand();
}


void ProcessInterruptCallProcessHandler::OnTimeoutFailed(PROCESS_INFORMATION& info) {
  myStopInterruptFlag.Interrupt();
  this->ProcessInterruptTimeoutHandler::OnTimeoutFailed(info);
}

