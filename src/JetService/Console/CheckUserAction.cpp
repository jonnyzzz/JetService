#include "StdAfx.h"
#include "CheckUserAction.h"
#include "CheckLogonAsServiceCommand.h"
#include "Logger.h"

const Logger LOG(L"CheckUserAction");

CheckUserAction::CheckUserAction() : UserConsoleAction(L"checkUser")
{
}


CheckUserAction::~CheckUserAction()
{
}


void CheckUserAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s %s", myName, GetUserArgumentsList());
  writer->Write      (L"      checks if user has enough rights to logon as service");
  writer->Write();
}


int CheckUserAction::ExecuteAction(const Argz* argz, UserSettings* settings) {
  CheckLogonAsServiceCommand check(settings);
  return static_cast<Command*>(&check)->executeCommand();  
}

