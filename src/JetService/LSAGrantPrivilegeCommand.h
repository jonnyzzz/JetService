#pragma once

#include "LSAUserCommand.h"


class LSAGrantPrivilegeCommand : public LSAUserCommand
{
public:
  LSAGrantPrivilegeCommand(const CreateServiceSettings* settings);
  virtual ~LSAGrantPrivilegeCommand();

public:
  virtual int executeCommand(LSA_HANDLE lsa, HANDLE userToken, PSID sid);
};

