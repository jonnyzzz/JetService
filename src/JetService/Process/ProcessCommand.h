#pragma once

#include "Command.h"
#include "ServiceSettings.h"
#include "InterruptHolder.h"
#include "Pipe.h"

class ProcessInterruptHandler {
public:
  ProcessInterruptHandler() {}
  virtual ~ProcessInterruptHandler() {}

public:
  ///ProcessCommand will call the method
  ///on and on while process is still running 
  ///after interrupt flag was set. 
  ///Implementation may take care with it
  virtual void InterruptProcess(PROCESS_INFORMATION& info) = 0;
};

class ProcessCommand : public Command, public InterruptHolder
{
public:
  ProcessCommand(const ExecutionSettings* settings, ProcessInterruptHandler* handler, InterruptHolder* interrupt = NULL);
  virtual ~ProcessCommand(void);

public:
  virtual int executeCommand();

private:  
  int DoStartProcessAsUser(HANDLE processUserHandle);
  int WaitForProcessAndCaptureOutput(ChildProcessOutHandle* handle, PROCESS_INFORMATION& processInfo);
  int WaitForProcessToExit(PROCESS_INFORMATION& processInfo);


  HANDLE CreateProcessStartUserToken();
  STARTUPINFO CreateProcessStartupInfo(ChildProcessInHandle* pstdin, ChildProcessOutHandle* pout);

private:
  const ExecutionSettings* const mySettings;  
  ProcessInterruptHandler* const myInterruptHandler;
};

