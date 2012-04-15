#pragma once

#include "ProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"



class ProcessInterruptTimeoutHandler : public ProcessInterruptHandler
{
public:
  ProcessInterruptTimeoutHandler(const ServiceTaskSettings* settings);
  virtual ~ProcessInterruptTimeoutHandler(void);

protected:
  const ServiceTaskSettings* const mySettings;

public:
  virtual void InterruptProcess(PROCESS_INFORMATION& info);

protected:
  virtual bool ExecuteInterruptAction(PROCESS_INFORMATION& info) = 0;
  virtual void OnTimeoutFailed(PROCESS_INFORMATION& info);

private:
  bool myActionFailed;
  ULONGLONG myTerminateSent;
  ProcessInterruptTerminateHandler myTerminate;
};

