#include "StdAfx.h"
#include "ServiceEventSTOPHandler.h"


ServiceEventSTOPHandler::ServiceEventSTOPHandler(void)
  : CALL_SERVICE_EVENT_HANDLER_BASE(SERVICE_CONTROL_STOP) 
{
}


ServiceEventSTOPHandler::~ServiceEventSTOPHandler(void)
{
}


DWORD ServiceEventSTOPHandler::HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) {  
  ServiceStatus* status = context->GetServiceStatus();
  status->SetStatus(StatusValue::STOPPED);
  return NO_ERROR;
} 