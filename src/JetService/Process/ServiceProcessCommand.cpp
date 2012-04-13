#include "StdAfx.h"
#include "ServiceProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessInterruptConsoleControlHandler.h"
#include "Logger.h"

const Logger LOG(L"ServiceProcessCommand");


ServiceProcessCommand::ServiceProcessCommand(const ServiceTaskSettings* settings, InterruptHolder* interrupt)
  : InterruptHolder(interrupt)
  , mySettings(settings)
{
}


ServiceProcessCommand::~ServiceProcessCommand(void)
{
}


int ServiceProcessCommand::executeCommand() {
  InterruptHolder interrupt;
  ProcessInterruptTerminateHandler inthndl;
  ProcessInterruptConsoleControlHandler intSignal(mySettings);

  ProcessInterruptHandler* handler;
  if (mySettings->getTerminateWaitTimeoutMilliseconds() <= 0) {
    LOG.LogDebug(L"Termination wait timeout is <= 0. Process will be terminated on service exit");
    handler = &inthndl;
  } else {
    LOG.LogDebugFormat(L"Termination wait timeout is %s milliseconds. Process will be terminated via Console Control Event first.");
    handler = &intSignal;
  }

  ProcessCommand cmd(mySettings, handler, &interrupt);
  return static_cast<Command*>(&cmd)->executeCommand();
}

