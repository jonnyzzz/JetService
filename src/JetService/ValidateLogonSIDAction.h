#pragma once

#include "CreateServiceSettingsAction.h"

class ValidateLogonSIDAction : public CreateServiceSettingsAction
{
public:
  ValidateLogonSIDAction(void);
  virtual ~ValidateLogonSIDAction(void);

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* taskSettings);

};

