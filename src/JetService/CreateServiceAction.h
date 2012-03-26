#pragma once

#include "SimpleConsoleAction.h"

class CreateServiceAction : public SimpleConsoleAction
{
public:
  CreateServiceAction();
  virtual ~CreateServiceAction();


public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const ServiceSettings* settings);

};

