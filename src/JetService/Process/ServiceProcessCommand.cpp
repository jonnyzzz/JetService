#include "StdAfx.h"
#include "ServiceProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessInterruptConsoleControlHandler.h"
#include "ProcessInterruptCallProcessHandler.h"
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
  LOG.LogDebug(L"starting process");

  long l =  mySettings->getTerminateWaitTimeoutMilliseconds();
  LOG.LogDebugFormat(L"stop wait timeout: %ld", l);

  ProcessInterruptTerminateHandler inthndl;  
  ProcessInterruptCallProcessHandler stopAction(mySettings);

  ProcessInterruptHandler* handler;
  if (mySettings->getStopCommand() != NULL) {    
    LOG.LogDebugFormat(L"Will call stop command. Termination wait timeout is %ld milliseconds.", mySettings->getTerminateWaitTimeoutMilliseconds());
    handler = &stopAction;
  } else {
    LOG.LogDebug(L"Process will be terminated on service exit");
    handler = &inthndl;
  }

  ProcessCommand cmd(mySettings, handler, this);
  return static_cast<Command*>(&cmd)->executeCommand();
}

