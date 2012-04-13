#include "StdAfx.h"
#include "ServiceEventSTOPHandler.h"
#include "ServiceTask.h"

ServiceEventSTOPHandler::ServiceEventSTOPHandler(void)
  : CALL_SERVICE_EVENT_HANDLER_BASE(SERVICE_CONTROL_STOP) 
{
}


ServiceEventSTOPHandler::~ServiceEventSTOPHandler(void)
{
}


DWORD ServiceEventSTOPHandler::HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) {  
  context->GetServiceTask()->CallStopSerive();
  return NO_ERROR;
} 