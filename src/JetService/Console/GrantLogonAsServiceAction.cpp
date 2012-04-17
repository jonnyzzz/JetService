#include "StdAfx.h"
#include "GrantLogonAsServiceAction.h"
#include "CheckAndGrantLogOnAsServiceCommand.h"
#include "Logger.h"

const Logger LOG(L"GrantLogonAsServiceAction");


GrantLogonAsServiceAction::GrantLogonAsServiceAction(void) : UserConsoleAction(L"grantLogon")
{
}


GrantLogonAsServiceAction::~GrantLogonAsServiceAction(void)
{
}


void GrantLogonAsServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s %s", myName, GetUserArgumentsList());
  writer->Write      (L"      grants user 'Logon as Service' privilege to user if needed");
  writer->Write();
}


int GrantLogonAsServiceAction::ExecuteAction(const Argz* argz, UserSettings* settings) {
  CheckAndGrantLogOnAsServiceCommand check(settings, true);
  return static_cast<Command*>(&check)->executeCommand();  
}

