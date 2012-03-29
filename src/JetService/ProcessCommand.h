#pragma once

#include "Command.h"
#include "ServiceSettings.h"


class ProcessCommand : public Command
{
public:
  ProcessCommand(const ServiceTaskSettings* settings);
  virtual ~ProcessCommand(void);

public:
  virtual int executeCommand();

protected:
  virtual bool IsInterrupted() const;

private:
  HANDLE CreateProcessToken();
  STARTUPINFO CreateProcessStartupInfo();

private:
  const ServiceTaskSettings* const mySettings;
};

