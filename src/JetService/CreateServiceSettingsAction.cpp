#include "StdAfx.h"
#include "CreateServiceSettingsAction.h"
#include "FileTaskSettings.h"
#include "SimpleServiceSettings.h"
#include "ServiceAction.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceSettingsAction");


CreateServiceSettingsAction::CreateServiceSettingsAction(const CString& commandName) 
  : SimpleConsoleAction(commandName)
{
}


CreateServiceSettingsAction::~CreateServiceSettingsAction()
{
}


class ValidateServiceTaskSettings : public FileTaskSettings {
public:

  ValidateServiceTaskSettings(const Argz* argz, CreateServiceSettingsAction*  action, const CreateServiceSettings* settings) 
    : FileTaskSettings(settings)
    , myBaseSettings(settings)
    , myAction(action)
    , myArgz(argz)
  {
  }
  virtual ~ValidateServiceTaskSettings() {}

public:
  virtual int executeCommand(const ServiceTaskSettings* settings) {
    return myAction->ExecuteAction(myArgz, myBaseSettings, settings);
  }

private:
  const CreateServiceSettings* const myBaseSettings;
  const Argz* const myArgz;
  CreateServiceSettingsAction* const myAction;
};



int CreateServiceSettingsAction::ExecuteAction(const Argz* az, const RunServiceSettings* baseSettings) {
  CString serviceCommand;
  {
    int ret = ServiceAction().GenerateServiceCommandLine(az, serviceCommand);
    if (ret != 0) return ret;
  }
  
  SimpleCreateServiceSettings settings(baseSettings, serviceCommand);

  if (az->HasArgument(L"/runAsSystem")) {
    settings.setRunAsSystem(true);
  } else {
    CString p;
    if (!az->GetNamedArgument(L"user", p)) {
      LOG.LogWarn(L"/user=<user> parameter must be specified.");
      return 1;
    }
    settings.setUserName(p);
    if (!az->GetNamedArgument(L"password", p)) {
      LOG.LogWarn(L"/password=<password> parameter must be specified.");
      return 1;
    }
    settings.setPassword(p);
  }

  CString p(L"true");
  az->GetNamedArgument(L"autorun", p);
  if (p.CompareNoCase(L"false") == 0) {
    settings.setAutostart(false);
  } 
  if (p.CompareNoCase(L"true") == 0) {
    settings.setAutostart(true);
  }

  ValidateServiceTaskSettings validate(az, this, &settings);

  return static_cast<Command*>(&validate)->executeCommand();
}

