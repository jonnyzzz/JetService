#pragma once

#include "ServiceSettings.h"
#include "LogonUserSIDCommand.h"
#include "LSAPolicyCommand.h"


class LSAUserCommand: public LogonUserSIDCommand
{
public:
  LSAUserCommand(const CreateServiceSettings* settings, const LSAPolicyRight& lsaRight);
  virtual ~LSAUserCommand();

public:  
  virtual int executeCommand(HANDLE userToken, PSID sid);
  virtual int executeCommand(LSA_HANDLE lsa, HANDLE userToken, PSID sid) = 0;

private:
  const CreateServiceSettings* mySettings;
  const LSAPolicyRight& myLSARight;
};

