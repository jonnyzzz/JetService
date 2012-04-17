#include "StdAfx.h"
#include "CheckLogonAsServiceCommand.h"
#include "Logger.h"

const Logger LOG(L"CheckLogonAsServiceCommand");


CheckLogonAsServiceCommand::CheckLogonAsServiceCommand(const UserSettings* settings) : LogonUserCommand(settings, LogonUserMode::AS_SERVICE) {
}


CheckLogonAsServiceCommand::~CheckLogonAsServiceCommand(void)
{
}

int CheckLogonAsServiceCommand::executeCommand(HANDLE userToken) {
  LOG.LogInfo(L"User logged in as service");
  return 0;
}
