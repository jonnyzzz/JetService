#pragma once

#include "CreateServiceSettingsAction.h"

class GrantServiceDACLAction : public CreateServiceSettingsAction
{
public:
  GrantServiceDACLAction();
  virtual ~GrantServiceDACLAction(void);

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task);
};

