#pragma once

#include "ServiceSettings.h"
#include "LogonUserCommand.h"
#include "LSAPolicyCommand.h"

class LogonUserSIDCommand: public LogonUserCommand
{
public:
  LogonUserSIDCommand(const CreateServiceSettings* settings);
  virtual ~LogonUserSIDCommand();

public:

  virtual int executeCommand(HANDLE userToken);
  virtual int executeCommand(HANDLE userToken, PSID sid) = 0;

private:
  int executeCommand(HANDLE userToken, PTOKEN_GROUPS groups, DWORD size);
  int executeCommandCopySID(HANDLE userToken, PSID sid);

private:
  const CreateServiceSettings* mySettings;
};

