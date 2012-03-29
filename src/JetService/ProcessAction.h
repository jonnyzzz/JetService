#pragma once

#include "SimpleConsoleAction.h"

class ProcessAction : public SimpleConsoleAction
{
public:
  ProcessAction();
  virtual ~ProcessAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const RunServiceSettings* settings);

};

