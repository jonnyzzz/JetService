#pragma once

#include "ProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"

class ProcessInterruptConsoleControlHandler : public ProcessInterruptTerminateHandler
{
public:
  ProcessInterruptConsoleControlHandler(const ServiceTaskSettings* settings);
  virtual ~ProcessInterruptConsoleControlHandler();

public:
  virtual void InterruptProcess(PROCESS_INFORMATION& info);

private:
  const ServiceTaskSettings* const mySettings;
  bool myCtrlEventFailed;
  ULONGLONG myTerminateSent;
};

