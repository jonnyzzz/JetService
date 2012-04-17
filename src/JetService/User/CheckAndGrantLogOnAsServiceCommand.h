#pragma once

#include "ServiceSettings.h"
#include "Command.h"

class CheckAndGrantLogOnAsServiceCommand : public Command
{
public:
  CheckAndGrantLogOnAsServiceCommand(const UserSettings* settings, bool checkFirst);
  ~CheckAndGrantLogOnAsServiceCommand(void);

public:
  virtual int executeCommand();

private:
  int CheckAndGrantLogOnAsServiceCommand::CheckServiceLogOn();

private:
  const UserSettings* const mySettings;
  const bool myCheckFirst;
};

