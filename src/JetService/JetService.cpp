// JetService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"
#include "ConsoleAction.h"
#include "SimpleConsoleWriter.h"
#include "DeleteServiceAction.h"
#include "CreateServiceAction.h"
#include "ProcessAction.h"
#include "ServiceAction.h"
#include "ConsoleCommandsRunner.h"
#include "ValidateCreateServiceAction.h"
#include "ValidateServiceTaskAction.h"
#include "ValidateLogonSIDAction.h"
#include "GrantLogonAsServiceAction.h"
#include "CheckUserAction.h"
#include "GrantServiceDACLAction.h"

#include "Logger.h"
const Logger LOG(L"main");

int _tmain(int argc, _TCHAR* argv[])
{
  SimpleConsoleWriter sConsole;
  ConsoleWriter* console = &sConsole;
  console->Write(L"Starting JetBrains JetService...");
  console->Write(L"Copyright (C) JetBrains GmbH 2012");
  console->Write();

  Argz az(argc, argv);

  DeleteServiceAction deleteAction;
  CreateServiceAction createAction;
  ServiceAction       serviceAction;
  ProcessAction       processAction;
  CheckUserAction     checkUserAction;
  GrantLogonAsServiceAction grantUserAction;
  GrantServiceDACLAction grantServiceAction;
  ValidateCreateServiceAction validateAction;
  ValidateServiceTaskAction validateTaskAction;
  ValidateLogonSIDAction validateSIDAction;
  

  ConsoleAction* actions[] = {
    &serviceAction,
    &createAction, 
    &deleteAction,
    &processAction,
    &checkUserAction,
    &grantUserAction,
    &grantServiceAction,
    &validateAction,
    &validateTaskAction,
    &validateSIDAction,
    NULL
  };

  return (ConsoleCommandsRunner(console, &az, actions)).executeCommand();
}

