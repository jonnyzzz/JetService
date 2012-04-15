#pragma once

#include "ProcessCommand.h"
#include "ProcessInterruptTimeoutHandler.h"

class ProcessInterruptConsoleControlHandler : public ProcessInterruptTimeoutHandler
{
public:
  ProcessInterruptConsoleControlHandler(const ServiceTaskSettings* settings);
  virtual ~ProcessInterruptConsoleControlHandler();

public:
  virtual bool ExecuteInterruptAction(PROCESS_INFORMATION& info);
};


