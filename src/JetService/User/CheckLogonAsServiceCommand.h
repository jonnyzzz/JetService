#pragma once

#include "LogonUserCommand.h"

class CheckLogonAsServiceCommand : public LogonUserCommand
{
public:
  CheckLogonAsServiceCommand(const UserSettings* settings);
  virtual ~CheckLogonAsServiceCommand();

public:
  virtual int executeCommand(HANDLE userToken);
};

