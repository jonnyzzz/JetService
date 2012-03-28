#pragma once

#include "ServiceEventContext.h"

class ServiceEventHandler
{
public:
  ServiceEventHandler(const CString& eventName, DWORD controlEventId);
  virtual ~ServiceEventHandler(void);

public:
  const CString& GetEventName() const;
  DWORD GetControlEventId() const;
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) = 0;

private:
  const CString myName;
  const DWORD myControlEventId;
};


#define CALL_SERVICE_EVENT_HANDLER_BASE(x) ServiceEventHandler(L#x, x)
