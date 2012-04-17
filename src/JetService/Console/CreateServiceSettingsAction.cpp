#include "StdAfx.h"
#include "CreateServiceSettingsAction.h"
#include "FileTaskSettings.h"
#include "SimpleServiceSettings.h"
#include "ServiceAction.h"
#include "LSAGrantPrivilegeCommand.h"
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
    int ret = ParseUserSettings(az, &settings);
    if (ret != 0) return ret;
    LOG.LogInfoFormat(L"Installing service under %s (domain=%s) account", settings.getUserName(), settings.getDomain());
  }

  settings.setAutostart(az->GetBooleanArgument(L"autostart", true) && az->GetBooleanArgument(L"autorun", true));

  return ExecuteAction(az, &settings, baseSettings);  
}



