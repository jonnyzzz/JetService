#include "StdAfx.h"
#include "ProcessInterruptCallProcessHandler.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessCommand.h"
#include "Logger.h"

const Logger LOG(L"ProcessInterruptCallProcessHandler");


ProcessInterruptCallProcessHandler::ProcessInterruptCallProcessHandler(const ServiceTaskSettings* settings)
  : mySettings(settings)
  , InterruptHolder()
  , myUseKill(false)
  , myStopCommandCalled(false)
{
}


ProcessInterruptCallProcessHandler::~ProcessInterruptCallProcessHandler()
{
}




void ProcessInterruptCallProcessHandler::InterruptProcess(PROCESS_INFORMATION& info) {
  const ExecutionSettings* stop = mySettings->getStopCommand();
  if (stop == NULL || myUseKill) {
    myServiceInterrupt.InterruptProcess(info);
    return;
  }

  if (myStopCommandCalled) {
    LOG.LogDebug(L"Stop command completed. Process should exit soon");
    return;
  }

  myStopCommandCalled = true;
  LOG.LogDebug(L"Executing stop command");
  ProcessCommand cmd(stop, &myStopInterrupt, this);
  static_cast<Command*>(&cmd)->executeCommand();
}

