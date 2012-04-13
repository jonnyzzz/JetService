#include "StdAfx.h"
#include "ServiceProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessInterruptConsoleControlHandler.h"


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
  ProcessInterruptConsoleControlHandler intSignal;

  ProcessCommand cmd(mySettings, &intSignal, &interrupt);
  return static_cast<Command*>(&cmd)->executeCommand();
}

