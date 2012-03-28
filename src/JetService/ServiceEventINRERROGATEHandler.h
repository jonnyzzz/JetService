#pragma once

#include "ServiceEventHandler.h"

class ServiceEventINRERROGATEHandler : public ServiceEventHandler
{
public:
  ServiceEventINRERROGATEHandler();
  virtual ~ServiceEventINRERROGATEHandler();

public: 
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);  
};

