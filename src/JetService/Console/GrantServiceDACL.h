#pragma once

#include "CreateServiceSettingsAction.h"

class GrantServiceDACL : public CreateServiceSettingsAction
{
public:
  GrantServiceDACL();
  virtual ~GrantServiceDACL(void);

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task);
};

