#pragma once

#include "ServiceSettingsAction.h"

class ValidateServiceTaskAction : public ServiceSettingsAction
{
public:
  ValidateServiceTaskAction();
  virtual ~ValidateServiceTaskAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const ServiceTaskSettings* taskSettings);

};

