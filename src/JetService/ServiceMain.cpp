#include "StdAfx.h"
#include "ServiceMain.h"
#include "ServiceEventHandlerCollection.h"
#include "Logger.h"

#include "ServiceEventContext.h"
#include "ServiceEventINRERROGATEHandler.h"
#include "ServiceEventSTOPHandler.h"

const Logger LOG(L"ServiceMain");

ServiceMain::ServiceMain(const RunServiceSettings* settings) 
  : mySettings(settings)  
{
}

ServiceMain::~ServiceMain() {
}


DWORD WINAPI GlobalHandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
  return ((ServiceEventHandlerCollection*)lpContext)->HandleServiceControlEvent(dwControl, dwEventType, lpEventData);
}


void ServiceMain::JetServiceMain(const Argz* serviceArgz) {
  LOG.LogDebug(L"Executing JetServiceMain");

  ServiceEventINRERROGATEHandler serviceEventINRERROGATEHandler;
  ServiceEventSTOPHandler serviceEventSTOPHandler;
  ServiceEventHandler* pHandlers[] = {  
    &serviceEventINRERROGATEHandler,
    &serviceEventSTOPHandler,
    NULL
  };

  ServiceEventContextEx context;
  ServiceEventHandlerCollection handlers(&context, pHandlers);
  
  SERVICE_STATUS_HANDLE myStatusHandle = RegisterServiceCtrlHandlerEx(mySettings->getServiceName(), GlobalHandlerEx, &handlers);
  if (myStatusHandle == NULL) {
    LOG.LogErrorFormat(L"Failed to RegisterServiceCtrlHandlerEx. %s", LOG.GetLastError());    
    return;
  }
  ServiceStatus status(myStatusHandle, handlers.GetSupportedControlEventsMask());
  context.SetServiceStatus(&status);
  LOG.LogDebug(L"RegisterServiceCtrlHandler completed");
  
  //TODO: call Start first and handle event to move into RUNNING
  status.SetStatus(StatusValue::RUNNING);
  
  status.WaitForExit();
    LOG.LogDebug(L"Stop event processes");
  return;  
}



/*
DWORD ServiceMain::HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData) {
  const Logger LOG(L"ServiceEvent");
  //see http://msdn.microsoft.com/en-us/library/windows/desktop/ms683241(v=vs.85).aspx
  switch(dwControl) 
  {  
  case SERVICE_CONTROL_INTERROGATE:
    LOG.LogInfo(L"SERVICE_CONTROL_INTERROGATE");
    ///Notifies a service to report its current status information to the service control manager.
    ///The handler should simply return NO_ERROR
    return NO_ERROR;

  case SERVICE_CONTROL_PARAMCHANGE:
    LOG.LogInfo(L"SERVICE_CONTROL_PARAMCHANGE");
    ///Notifies a service that service-specific startup parameters have changed. 
    ///The service should reread its startup parameters.
    ///TODO:
    return ERROR_CALL_NOT_IMPLEMENTED;

  case SERVICE_CONTROL_PRESHUTDOWN:
    LOG.LogInfo(L"SERVICE_CONTROL_PRESHUTDOWN");
    ///handle it to have more time to safely shutdown
    ///not supported for XP and Server 2003
    return HandlerEx(SERVICE_CONTROL_STOP, 0, 0);

  case SERVICE_CONTROL_SHUTDOWN:
    LOG.LogInfo(L"SERVICE_CONTROL_SHUTDOWN");
    ///handle system shutdown.
    ///TODO: should this be synchronized shutdown? 
    return HandlerEx(SERVICE_CONTROL_STOP, 0, 0);

  case SERVICE_CONTROL_POWEREVENT:
    LOG.LogInfo(L"SERVICE_CONTROL_POWEREVENT");
    ///called to power settings change
    return ERROR_CALL_NOT_IMPLEMENTED;

  case SERVICE_CONTROL_SESSIONCHANGE:
    LOG.LogInfo(L"SERVICE_CONTROL_SESSIONCHANGE");
    ///called on a session change event
    return ERROR_CALL_NOT_IMPLEMENTED;

  case SERVICE_CONTROL_TIMECHANGE:
    LOG.LogInfo(L"SERVICE_CONTROL_TIMECHANGE");
    ///called on system time change
    return ERROR_CALL_NOT_IMPLEMENTED;

  case SERVICE_CONTROL_STOP: 
    LOG.LogInfo(L"SERVICE_CONTROL_STOP");
    ///Notifies a service that it should stop
    ///If a service accepts this control code, it must stop upon receipt and return NO_ERROR. 
    ///ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR);

    // Signal the service to stop.
    SetEvent(myStopEvent);
    return NO_ERROR;

  default:
    return ERROR_CALL_NOT_IMPLEMENTED;
   }   
}
*/
