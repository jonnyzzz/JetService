#pragma once

#include "ServiceCommand.h"

class DeleteServiceCommand : public ServiceCommand
{
public:
  DeleteServiceCommand(const ServiceSettings* settings);
  virtual ~DeleteServiceCommand(void);

public:
  virtual int executeCommand(SC_HANDLE serviceControlManager, SC_HANDLE service);

};

