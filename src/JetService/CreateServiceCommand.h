#pragma once

#include "ServiceSettings.h"
#include "ServiceControlManagerCommand.h"


class CreateServiceCommand : public ServiceControlManagerCommand
{
public:
  CreateServiceCommand(const CreateServiceSettings* settings);
  virtual ~CreateServiceCommand();

public:
  virtual int executeCommand(SC_HANDLE scm);

private:
  const CreateServiceSettings* mySettings;

};

