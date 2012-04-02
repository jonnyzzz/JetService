#pragma once

#include "CreateServiceSettingsAction.h"

class CreateServiceAction : public CreateServiceSettingsAction
{
public:
  CreateServiceAction();
  virtual ~CreateServiceAction();


public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task);

};

