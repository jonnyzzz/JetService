#pragma once

#include "Command.h"
#include "ServiceSettings.h"
#include "InterruptHolder.h"


class ProcessCommand : public Command
{
public:
  ProcessCommand(const ServiceTaskSettings* settings, const InterruptHolder* interrupt);
  virtual ~ProcessCommand(void);

public:
  virtual int executeCommand();

private:
  bool IsInterrupted();
  HANDLE CreateProcessToken();
  STARTUPINFO CreateProcessStartupInfo();

private:
  const ServiceTaskSettings* const mySettings;
  const InterruptHolder* const myInterrupt;
};

