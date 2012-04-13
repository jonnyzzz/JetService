#pragma once

#include "ProcessCommand.h"
#include "InterruptHolder.h"

class ProcessInterruptTerminateHandler : public ProcessInterruptHandler, private InterruptHolder
{
public:
  ProcessInterruptTerminateHandler(void);
  virtual ~ProcessInterruptTerminateHandler(void);

public:
  virtual void InterruptProcess(PROCESS_INFORMATION& info);
};

