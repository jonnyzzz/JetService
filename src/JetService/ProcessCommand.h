#pragma once

#include "Command.h"
#include "ServiceSettings.h"
#include "InterruptHolder.h"
#include "Pipe.h"


class ProcessCommand : public Command, protected InterruptHolder
{
public:
  ProcessCommand(const ServiceTaskSettings* settings, const InterruptHolder* interrupt);
  virtual ~ProcessCommand(void);

public:
  virtual int executeCommand();

private:  
  HANDLE CreateProcessToken();
  STARTUPINFO CreateProcessStartupInfo(ChildProcessHandle* pstdin, ChildProcessHandle* pstdout, ChildProcessHandle* pstderr);

private:
  const ServiceTaskSettings* const mySettings;  
};

