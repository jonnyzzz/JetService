#include "StdAfx.h"
#include "ServiceEventHandlerCollection.h"
#include "Logger.h"

const Logger LOG(L"ServiceEventHandlerCollection");

ServiceEventHandlerCollection::ServiceEventHandlerCollection(ServiceEventContext* context, ServiceEventHandler** handlers)
  : myContext(context)
  , myHandlers(handlers)
{
}


ServiceEventHandlerCollection::~ServiceEventHandlerCollection(void)
{
}


DWORD ServiceEventHandlerCollection::GetSupportedControlEventsMask() const {
  DWORD mask = 0;
  for(ServiceEventHandler** p = myHandlers; *p != NULL; p++) {
    mask |= (*p)->GetControlEventId();
  }
  return mask;
}
 
DWORD ServiceEventHandlerCollection::HandleServiceControlEvent(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) const {  
  if (!myContext->IsValid()) {
    LOG.LogError(L"Context is not fully initialized. Event handlers will not be called");
    return ERROR_CALL_NOT_IMPLEMENTED;
  }

  for(ServiceEventHandler** p = myHandlers; *p != NULL; p++) {
    ServiceEventHandler* handler = *p;

    if (dwControl == handler->GetControlEventId()) {
      LOG.LogDebugFormat(L"Processing service event %d with %s", dwControl, handler->GetEventName());
      return handler->HandleServiceEvent(myContext, dwControl, dwEventType, lpEventData);
    }    
  }
  
  LOG.LogDebugFormat(L"Unknown service event: %d", dwControl);
  return ERROR_CALL_NOT_IMPLEMENTED;
}
