// JetService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"
#include "ConsoleAction.h"
#include "SimpleConsoleWriter.h"
#include "DeleteServiceAction.h"
#include "CreateServiceAction.h"
#include "ServiceAction.h"
#include "ConsoleCommandsRunner.h"

#include "Logger.h"
const Logger LOG(L"main");


int _tmain(int argc, _TCHAR* argv[])
{
  SimpleConsoleWriter sConsole;
  ConsoleWriter* console = &sConsole;

  Argz az(argc, argv);

  if (az.HasArgument(L"/debug")) {
    Logger::SetSuverity(LogSDebug);
    LOG.LogDebug(L"Debug logging is enabled");
  } else {
    Logger::SetSuverity(LogSInfo);
  }

  console->Write(L"Starting JetService...");
  console->Write(L"Copyright (C) JetBrains GmbH 2012");
  console->Write();


  //TODO: move initialization into main() method to avoid 
  //TODO: static constats initialization issues
  DeleteServiceAction deleteAction;
  CreateServiceAction createAction;
  ServiceAction       serviceAction;

  ConsoleAction* actions[] = {
    &serviceAction,
    &createAction, 
    &deleteAction,
    NULL
  };

  return (ConsoleCommandsRunner(console, &az, actions)).executeCommand();
}

