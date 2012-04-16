#include "StdAfx.h"
#include "ServiceEventShutdownHandler.h"
#include "ServiceTask.h"
#include "Logger.h"

const Logger LOG(L"ServiceEventShutdownHandler");


ServiceEventShutdownHandler::ServiceEventShutdownHandler()
  : CALL_SERVICE_EVENT_HANDLER_BASE(SERVICE_CONTROL_SHUTDOWN) 
{
}


ServiceEventShutdownHandler::~ServiceEventShutdownHandler(void)
{
}

DWORD ServiceEventShutdownHandler::HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) {
  LOG.LogInfo(L"System shutdown notification received");
  context->GetServiceTask()->CallStopSerive();
  return 0;
}

