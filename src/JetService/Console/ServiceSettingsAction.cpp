#include "StdAfx.h"
#include "ServiceSettingsAction.h"
#include "FileTaskSettings.h"
#include "SimpleServiceSettings.h"
#include "ServiceAction.h"
#include "Logger.h"

const Logger LOG(L"ServiceSettingsAction");


ServiceSettingsAction::ServiceSettingsAction(const CString& commandName)
  : SimpleConsoleAction(commandName)
{
}


ServiceSettingsAction::~ServiceSettingsAction()
{
}


class ValidateServiceTaskSettings : public FileTaskSettings {
public:

  ValidateServiceTaskSettings(const Argz* argz, ServiceSettingsAction*  action, const ServiceSettings* settings) 
    : FileTaskSettings(settings)
    , myAction(action)
    , myArgz(argz)
  {
  }
  virtual ~ValidateServiceTaskSettings() {}

public:
  virtual int executeCommand(const ServiceTaskSettings* settings) {
    return myAction->ExecuteAction(myArgz, settings);
  }

private:
  const Argz* const myArgz;
  ServiceSettingsAction* const myAction;
};


int ServiceSettingsAction::ExecuteAction(const Argz* az, const ServiceSettings* baseSettings) {
  ValidateServiceTaskSettings validate(az, this, baseSettings);
  return static_cast<Command*>(&validate)->executeCommand();
}

