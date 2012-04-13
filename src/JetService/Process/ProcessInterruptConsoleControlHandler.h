#pragma once

#include "ProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"

class ProcessInterruptConsoleControlHandler : public ProcessInterruptTerminateHandler
{
public:
  ProcessInterruptConsoleControlHandler(void);
  virtual ~ProcessInterruptConsoleControlHandler(void);

public:
  virtual void InterruptProcess(PROCESS_INFORMATION& info);

private:
  bool myCtrlEventFailed;
  ULONGLONG myTerminateSent;
};

