#include "StdAfx.h"
#include "ServiceEventINRERROGATEHandler.h"

ServiceEventINRERROGATEHandler::ServiceEventINRERROGATEHandler(void)
  : CALL_SERVICE_EVENT_HANDLER_BASE(SERVICE_CONTROL_INTERROGATE) 
{
}


ServiceEventINRERROGATEHandler::~ServiceEventINRERROGATEHandler(void)
{
}

DWORD ServiceEventINRERROGATEHandler::HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) {
  return NO_ERROR;
} 