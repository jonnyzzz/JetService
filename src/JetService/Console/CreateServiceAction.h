#pragma once

#include "CreateServiceCheckAccountAction.h"

class CreateServiceAction : public CreateServiceCheckAccountAction
{
public:
  CreateServiceAction();
  virtual ~CreateServiceAction();


public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task);

private:
  static const CString KEY_DACL;
};

