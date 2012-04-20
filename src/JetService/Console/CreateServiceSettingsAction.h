#pragma once

#include "ServiceSettingsAction.h"
#include "UserConsoleAction.h"

class CreateServiceSettingsAction : public ServiceSettingsAction, protected UserSettingsParser
{
public:
  CreateServiceSettingsAction(const CString& commandName);
  virtual ~CreateServiceSettingsAction();

public:
  virtual int ExecuteAction(const Argz* argz, const ServiceTaskSettings* task);

  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task) = 0;

protected:
  static const CString KEY_AUTORUN;
};

