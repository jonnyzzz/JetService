#pragma once

#include "ServiceSettingsAction.h"

class CreateServiceSettingsAction : public ServiceSettingsAction
{
public:
  CreateServiceSettingsAction(const CString& commandName);
  virtual ~CreateServiceSettingsAction();

public:
  virtual int ExecuteAction(const Argz* argz, const ServiceTaskSettings* task);

  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task) = 0;
};

