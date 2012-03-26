// JetService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"
#include "ConsoleAction.h"
#include "DeleteServiceAction.h"

#include "Logger.h"
const Logger LOG(L"main");

DeleteServiceAction deleteAction;

ConsoleAction* actions[] = {
    &deleteAction,
    NULL
};

int usage(const Argz& az) {


  return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
  Argz az(argc, argv);

  if (az.HasArgument(L"/debug")) {
    Logger::SetSuverity(LogSDebug);
  } else {
    Logger::SetSuverity(LogSInfo);
  }

  LOG.LogInfo(L"Starting JetService...");
  LOG.LogInfo(L"Copyright (C) JetBrains GmbH 2012");
  LOG.LogInfo(L"");

  if (az.GetArgumentCount() < 1) {
    LOG.LogWarn(L"Incorrect usage.");
    return usage(az);
  }

  CString command = az.GetArgument(0);
  Argz subArgz = az.SkipFirstParameter();
  for(ConsoleAction** p = actions; *p != NULL; p++) {
    ConsoleAction* action = *p;
    if (action->GetCommandName() == command) {
      return action->ExecuteAction(&subArgz);
    }
  }

  LOG.LogWarnFormat(L"Unknown command: %s", command);
  return usage(az);
}

