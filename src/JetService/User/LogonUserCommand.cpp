#include "StdAfx.h"
#include "LogonUserCommand.h"
#include "Logger.h"

const Logger LOG(L"LogonUserCommand");

const LogonUserMode LogonUserMode::NETWORK = LogonUserMode(L"Network", LOGON32_LOGON_NETWORK);
const LogonUserMode LogonUserMode::AS_SERVICE = LogonUserMode(L"Logon as service", LOGON32_LOGON_SERVICE);

LogonUserCommand::LogonUserCommand(const UserSettings* settings, const LogonUserMode& mode)
  : mySettings(settings)
  , myMode(mode)
{
}


LogonUserCommand::~LogonUserCommand()
{
}


int LogonUserCommand::executeCommand() {
  HANDLE userToken;
  if (0 == LogonUser(
    mySettings->getUserName(),
    mySettings->getDomain(),
    mySettings->getPassword(),
    myMode,
    LOGON32_PROVIDER_DEFAULT,
    &userToken)) {
      DWORD err = GetLastError(); 
      CString msg = CreateFormatted(L"Failed to Logon user %s\\%s.", mySettings->getDomain(), mySettings->getUserName());      
      switch (err) {
      case ERROR_PRIVILEGE_NOT_HELD:
        LOG.LogWarnFormat(L"%s Not enough rights to check user account. SE_TCB_NAME is not given. %s", msg, LOG.GetErrorText(err));
        return 0;
      default:
        LOG.LogErrorFormat(L"%s %s", msg, LOG.GetErrorText(err));
        return 1;
      }
  }
  LOG.LogDebugFormat(L"User %s logged in as %s", mySettings->getUserName(), (CString)myMode);

  int ret = executeCommand(userToken);
  CloseHandle(userToken);
  return ret;
}

