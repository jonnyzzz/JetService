#include "StdAfx.h"
#include "ServiceEventPreShutdownHandler.h"
#include "ServiceTask.h"
#include "Logger.h"

const Logger LOG(L"ServiceEventPreShutdownHandler");



ServiceEventPreShutdownHandler::ServiceEventPreShutdownHandler(void)
   : CALL_SERVICE_EVENT_HANDLER_BASE(SERVICE_CONTROL_PRESHUTDOWN) 
{
}

ServiceEventPreShutdownHandler::~ServiceEventPreShutdownHandler(void)
{
}

DWORD ServiceEventPreShutdownHandler::HandleServiceEvent(ServiceEventContext* context, DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) {
  LOG.LogInfo(L"System pre-shutdown notification received");
  context->GetServiceTask()->CallStopSerive();
  return 0;
}

