#pragma once

#include "ServiceEventHandler.h"

class ServiceEventPreShutdownHandler : public ServiceEventHandler
{
public:
  ServiceEventPreShutdownHandler();
  virtual ~ServiceEventPreShutdownHandler();

public:
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);  
};
