#pragma once

#include "SimpleConsoleAction.h"
#include "JetConsoleControlHandler.h"
#include "InterruptHolder.h"

class ProcessAction : public SimpleConsoleAction, private DelegateConsoleControlHandler<ProcessAction>, private InterruptHolder
{
public:
  ProcessAction();
  virtual ~ProcessAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const ServiceSettings* settings);

public:
  int ExecuteActionWithSettings(const ServiceTaskSettings* settings);
  int ExecuteActionWithConsoleHandler(const ServiceTaskSettings* settings);

  BOOL ConsoleControlHandler(DWORD dwCtrlType);

};

