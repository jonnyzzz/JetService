#pragma once

#include "CreateServiceSettingsAction.h"

class ValidateCreateServiceAction : public CreateServiceSettingsAction
{
public:
  ValidateCreateServiceAction();
  virtual ~ValidateCreateServiceAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* taskSettings);

};

