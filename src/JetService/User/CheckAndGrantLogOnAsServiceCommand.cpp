#include "StdAfx.h"
#include "CheckAndGrantLogOnAsServiceCommand.h"
#include "CheckLogonAsServiceCommand.h"
#include "LSAGrantPrivilegeCommand.h"
#include "Logger.h"

const Logger LOG(L"CheckAndGrantLogOnAsServiceCommand");

CheckAndGrantLogOnAsServiceCommand::CheckAndGrantLogOnAsServiceCommand(const UserSettings* settings, bool checkFirst) 
  : mySettings(settings)  
  , myCheckFirst(checkFirst)
{
}


CheckAndGrantLogOnAsServiceCommand::~CheckAndGrantLogOnAsServiceCommand()
{
}


int CheckAndGrantLogOnAsServiceCommand::CheckServiceLogOn() {
  CheckLogonAsServiceCommand check(mySettings);
  return static_cast<Command*>(&check)->executeCommand();  
}


int CheckAndGrantLogOnAsServiceCommand::executeCommand() {
  int ret;
  if (myCheckFirst) {
    ret = CheckServiceLogOn();
    if (ret == 0) {
      LOG.LogInfo(L"User already has 'Logon as service' privilege");
      return 0;
    }
  }

  LSAGrantPrivilegeCommand cmd(mySettings);
  ret = static_cast<Command*>(&cmd)->executeCommand();
  if (ret == 0) {        
    ret = CheckServiceLogOn();
  }

  if (ret == 0) {
    LOG.LogInfo(L"Gave user Logon as service' privilege");
    return 0;
  } else {
    LOG.LogInfo(L"Failed to give user enough rights to run as server");
    return 1;
  }
}