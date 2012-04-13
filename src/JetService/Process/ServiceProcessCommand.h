#pragma once

#include "Command.h"
#include "ServiceSettings.h"
#include "InterruptHolder.h"

class ServiceProcessCommand : public Command, public InterruptHolder
{
public:
  ServiceProcessCommand(const ServiceTaskSettings* settings, InterruptHolder* interrupt = NULL);
  virtual ~ServiceProcessCommand(void);

public:
  virtual int executeCommand();

private:
  const ServiceTaskSettings* const mySettings;
};

