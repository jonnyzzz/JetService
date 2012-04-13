#pragma once

#include "ServiceEventHandler.h"

class ServiceEventSTOPHandler : public ServiceEventHandler
{
public:
  ServiceEventSTOPHandler(void);
  virtual ~ServiceEventSTOPHandler(void);

public: 
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);  

};

