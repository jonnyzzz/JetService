#pragma once

#include "ServiceEventHandler.h"

class ServiceEventHandlerCollection
{
public:
  //NULL-terminated list of handlers
  ServiceEventHandlerCollection(ServiceEventContext* context, ServiceEventHandler** handlers);
  virtual ~ServiceEventHandlerCollection(void);

public:
  DWORD GetSupportedControlEventsMask() const;
  DWORD HandleServiceControlEvent(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) const;

private:
  ServiceEventContext* const myContext;
  ServiceEventHandler** const myHandlers;
  
};

