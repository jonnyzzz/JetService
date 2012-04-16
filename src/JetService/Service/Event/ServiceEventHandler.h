#pragma once

#include "ServiceEventContext.h"

///Implementations of handler must be registered in ServiceMain.cpp
class ServiceEventHandler
{
public:
  ServiceEventHandler(const CString& eventName, DWORD controlEventId);
  virtual ~ServiceEventHandler(void);

public:
  const CString& GetEventName() const;
  DWORD GetControlEventId() const;

public:
  ///Implementation of this method should work fast
  ///If extra work is needed, it should create thread to 
  ///perform it.
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) = 0;

private:
  const CString myName;
  const DWORD myControlEventId;
};


#define CALL_SERVICE_EVENT_HANDLER_BASE(x) ServiceEventHandler(L#x, x)

#include "Logger.h"

#define DECLARE_LOGGING_HANDLER(name, evt, msg)                         \
class name : public ServiceEventHandler {                               \
public:                                                                 \
  name() : CALL_SERVICE_EVENT_HANDLER_BASE(evt) {}                      \
  virtual ~ name() {}                                                    \
                                                                        \
public:                                                                 \
  virtual DWORD HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) { \
    Logger(L#name).LogInfo(msg);                                       \
    return 0;                                                           \
  }                                                                     \
};                                                                      \
                                                                        \

DECLARE_LOGGING_HANDLER(ServiceEventPowerHandler, SERVICE_CONTROL_POWEREVENT, L"Power event recieved");
DECLARE_LOGGING_HANDLER(ServiceEventSessionHandler, SERVICE_CONTROL_SESSIONCHANGE, L"Session change event recieved");
