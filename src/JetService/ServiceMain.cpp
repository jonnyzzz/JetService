#include "StdAfx.h"
#include "ServiceMain.h"
#include "ServiceEventHandlerCollection.h"
#include "Logger.h"

#include "ServiceEventContext.h"
#include "ThreadedServiceTask.h"
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
  ThreadedServiceTask task(&context);

  context.SetServiceSettings(mySettings);
  context.SetServiceTask(&task);
  context.SetServiceStatus(&status);
  if (!context.IsValid()) {
    LOG.LogError(L"Failed to initialize ServiceContext. IsValid() returned false");
    //TODO: Introduce Stopped-error status here!
    status.SetStatus(StatusValue::STOPPED);
    return;
  }
  LOG.LogDebug(L"RegisterServiceCtrlHandler completed");
    
  //initialize status
  status.SetStatus(StatusValue::STARTING);
  //set task decide what to call next
  task.CallStartService();
  
  //wait for the end.
  status.WaitForExit();
  LOG.LogDebug(L"Stop event processes");
  
  return;  
}



