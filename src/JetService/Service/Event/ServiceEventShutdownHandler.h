#pragma once

#include "ServiceEventHandler.h"

class ServiceEventShutdownHandler : public ServiceEventHandler
{
public:
  ServiceEventShutdownHandler();
  virtual ~ServiceEventShutdownHandler();

public:
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);  

};

