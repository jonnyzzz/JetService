#pragma once

#include "Command.h"
#include "ServiceSettings.h"
#include "InterruptHolder.h"
#include "Pipe.h"


class ProcessCommand : public Command, public InterruptHolder
{
public:
  ProcessCommand(const ServiceTaskSettings* settings, InterruptHolder* interrupt = NULL);
  virtual ~ProcessCommand(void);

public:
  virtual int executeCommand();

private:  
  HANDLE CreateProcessToken();
  STARTUPINFO CreateProcessStartupInfo(ChildProcessInHandle* pstdin, ChildProcessOutHandle* pout);

private:
  const ServiceTaskSettings* const mySettings;  
};

