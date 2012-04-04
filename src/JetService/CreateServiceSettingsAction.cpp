#include "StdAfx.h"
#include "CreateServiceSettingsAction.h"
#include "FileTaskSettings.h"
#include "SimpleServiceSettings.h"
#include "ServiceAction.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceSettingsAction");


CreateServiceSettingsAction::CreateServiceSettingsAction(const CString& commandName) 
  : ServiceSettingsAction(commandName)
{
}


CreateServiceSettingsAction::~CreateServiceSettingsAction()
{
}


int CreateServiceSettingsAction::ExecuteAction(const Argz* az, const ServiceTaskSettings* baseSettings) {
  CString serviceCommand;
  {
    int ret = ServiceAction().GenerateServiceCommandLine(az, serviceCommand);
    if (ret != 0) return ret;
  }
  
  SimpleCreateServiceSettings settings(baseSettings, serviceCommand);

  if (az->HasArgument(L"/runAsSystem")) {
    settings.setRunAsSystem(true);
    LOG.LogInfo(L"Installing service unser LOCAL_SYSTEM account");
  } else {
    CString p;
    if (!az->GetNamedArgument(L"user", p)) {
      LOG.LogError(L"/user=<user> parameter must be specified.");
      return 1;
    }
    settings.setUserName(p);
    
    if (!az->GetNamedArgument(L"password", p)) {
      LOG.LogError(L"/password=<password> parameter must be specified.");
      return 1;
    }
    settings.setPassword(p);

    if (~az->GetNamedArgument(L"domain", p)) {
      LOG.LogWarnFormat(L"Domain not specified. Will use localhost");
      settings.setDomain(L".");
    } else {
      settings.setDomain(p);
    }

    LOG.LogInfoFormat(L"Installing service under %s (domain=%s) account", settings.getUserName(), settings.getDomain());
  }

  CString p(L"true");
  az->GetNamedArgument(L"autorun", p) || az->GetNamedArgument(L"autostart", p);
  if (p.CompareNoCase(L"false") == 0) {
    settings.setAutostart(false);
  } 
  if (p.CompareNoCase(L"true") == 0) {
    settings.setAutostart(true);
  }

  return ExecuteAction(az, &settings, baseSettings);  
}

