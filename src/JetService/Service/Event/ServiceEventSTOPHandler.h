#pragma once

#include "ServiceEventHandler.h"

class ServiceEventSTOPHandler : public ServiceEventHandler
{
public:
  ServiceEventSTOPHandler();
  virtual ~ServiceEventSTOPHandler();

public: 
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);  

};

