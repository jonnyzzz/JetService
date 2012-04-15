#pragma once

#include "ProcessCommand.h"
#include "ServiceSettings.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessInterruptTimeoutHandler.h"

class ProcessInterruptCallProcessHandler : public ProcessInterruptTimeoutHandler
{
public:
  ProcessInterruptCallProcessHandler(const ServiceTaskSettings* settings);
  virtual ~ProcessInterruptCallProcessHandler();

public: 
  virtual bool ExecuteInterruptAction(PROCESS_INFORMATION& info);
  virtual void OnTimeoutFailed(PROCESS_INFORMATION& info);

private:
  ProcessInterruptTerminateHandler myStopInterruptAction;
  InterruptHolder myStopInterruptFlag;
};

