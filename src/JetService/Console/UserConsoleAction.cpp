#include "StdAfx.h"
#include "UserConsoleAction.h"
#include "SimpleServiceSettings.h"
#include "Logger.h"


int UserSettingsParser::ParseUserSettings(const Argz* az, SimpleUserSettings* settings) {
  const Logger LOG(L"UserSettingsParser");

  CString p;
  if (!az->GetNamedArgument(L"user", p)) {
    LOG.LogError(L"/user=<user> parameter must be specified.");
    return 1;
  }
  settings->setUserName(p);
    
  if (!az->GetNamedArgument(L"password", p)) {
    LOG.LogError(L"/password=<password> parameter must be specified.");
    return 1;
  }
  settings->setPassword(p);

  if (!az->GetNamedArgument(L"domain", p)) {
    LOG.LogWarnFormat(L"Domain not specified. Will use localhost");
    settings->setDomain(L".");
  } else {
    settings->setDomain(p);
  }

  return 0;
}

CString UserSettingsParser::GetUserArgumentsList() {
  return L"/user=<user> [/domain=<domain>] /password=<password>";
}


UserConsoleAction::UserConsoleAction(const CString& commandName) : ConsoleAction(commandName)
{
}


UserConsoleAction::~UserConsoleAction()
{
}

int UserConsoleAction::ExecuteAction(const Argz* argz) {
  SimpleUserSettings settings;
  int ret = ParseUserSettings(argz, &settings);
  if (ret != 0) return ret;
  return ExecuteAction(argz, &settings);
}




