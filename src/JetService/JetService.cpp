// JetService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"
#include "ConsoleAction.h"
#include "SimpleConsoleWriter.h"
#include "DeleteServiceAction.h"

#include "Logger.h"
const Logger LOG(L"main");


DeleteServiceAction deleteAction;


ConsoleAction* actions[] = {
    &deleteAction,
    NULL
};

int usage(const Argz& az, ConsoleWriter* writer) {
  writer->Write();
  writer->Write(L"Usage:");
  writer->WriteFormat(L"  %s <command>", az.GetExecutableName());
  writer->Write();
  writer->Write(L"  where <command> could be one of the following: ");
  for(ConsoleAction** p = actions; *p != NULL; p++) {    
    (*p)->PrintUsage(writer);
    writer->Write();
  }

  return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
  SimpleConsoleWriter sConsole;
  ConsoleWriter* console = &sConsole;

  Argz az(argc, argv);

  if (az.HasArgument(L"/debug")) {
    Logger::SetSuverity(LogSDebug);
  } else {
    Logger::SetSuverity(LogSInfo);
  }

  console->Write(L"Starting JetService...");
  console->Write(L"Copyright (C) JetBrains GmbH 2012");
  console->Write();

  if (az.GetArgumentCount() < 1) {
    LOG.LogWarn(L"Incorrect usage.");
    return usage(az, console);
  }

  CString& command = az.GetArgument(0);
  Argz& subArgz = az.SkipFirstParameter();

  for(ConsoleAction** p = actions; *p != NULL; p++) {
    ConsoleAction* action = *p;
    if (action->GetCommandName() == command) {
      return action->ExecuteAction(&subArgz);
    }
  }

  LOG.LogWarnFormat(L"Unknown command: %s", command);
  return usage(az, console);
}

