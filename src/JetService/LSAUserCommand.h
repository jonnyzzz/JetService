#pragma once

#include "ServiceSettings.h"
#include "LSAPolicyCommand.h"


class LSAUserCommand : public LSAPolicyCommand
{
public:
  LSAUserCommand(const CreateServiceSettings* settings, const LSAPolicyRight& lsaRight);
  virtual ~LSAUserCommand();

public:  
  virtual int executeCommand(LSA_HANDLE handle);
  virtual int executeCommand(LSA_HANDLE lsa, PSID sid) = 0;

protected:
  const CreateServiceSettings* mySettings;
  const LSAPolicyRight& myLSARight;
};

