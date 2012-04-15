#pragma once

#include "ProcessCommand.h"
#include "ServiceSettings.h"
#include "ProcessInterruptTerminateHandler.h"

class ProcessInterruptCallProcessHandler : public ProcessInterruptHandler, private InterruptHolder
{
public:
  ProcessInterruptCallProcessHandler(const ServiceTaskSettings* settings);
  virtual ~ProcessInterruptCallProcessHandler();

public: 
  virtual void InterruptProcess(PROCESS_INFORMATION& info);

public:
  DWORD ThreadProcess();

private:
  bool myUseKill;
  bool myStopCommandCalled;

  ProcessInterruptTerminateHandler myServiceInterrupt;
  ProcessInterruptTerminateHandler myStopInterrupt;
  const ServiceTaskSettings* const mySettings;

  HANDLE myStopProcessThread;
  DWORD myStopProcessThreadId;
};

