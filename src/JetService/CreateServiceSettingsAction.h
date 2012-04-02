#pragma once

#include "SimpleConsoleAction.h"

class CreateServiceSettingsAction : public SimpleConsoleAction
{
public:
  CreateServiceSettingsAction(const CString& commandName);
  virtual ~CreateServiceSettingsAction();

public:
  virtual int ExecuteAction(const Argz* argz, const RunServiceSettings* settings);
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task) = 0;
};

