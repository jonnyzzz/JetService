#pragma once
#include "SimpleConsoleAction.h"

class ServiceAction : public SimpleConsoleAction
{
public:
  ServiceAction();
  virtual ~ServiceAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const ServiceSettings* settings);

public:
  static const CString JetServiceCommand;

};
