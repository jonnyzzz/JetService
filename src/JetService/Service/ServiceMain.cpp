#include "StdAfx.h"
#include "ServiceMain.h"
#include "ServiceEventHandlerCollection.h"
#include "Logger.h"

#include "ServiceEventContext.h"
#include "ServiceExecuteProcessTask.h"
#include "ServiceEventINRERROGATEHandler.h"
#include "ServiceEventSTOPHandler.h"
#include "ServiceEventPreShutdownHandler.h"
#include "ServiceEventShutdownHandler.h"

const Logger LOG(L"ServiceMain");

ServiceMain::ServiceMain(const ServiceSettings* settings) 
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
  ServiceEventPreShutdownHandler preShutdownHandler;
  ServiceEventShutdownHandler shutdownHandler;
  ServiceEventPowerHandler powerHandler;
  ServiceEventSessionHandler sessionHander;

  ServiceEventHandler* pHandlers[] = {  
    &serviceEventINRERROGATEHandler,
    &serviceEventSTOPHandler,
    &preShutdownHandler,
    &shutdownHandler,
    &powerHandler,
    &sessionHander,
    NULL
  };

  ServiceEventContextEx context;
  ServiceEventHandlerCollection handlers(&context, pHandlers);
  
  JetServiceMain(serviceArgz, &context, &handlers);
}

void ServiceMain::JetServiceMain(const Argz* serviceArgz, ServiceEventContextEx* context, ServiceEventHandlerCollection* handlers) {
  SERVICE_STATUS_HANDLE myStatusHandle = RegisterServiceCtrlHandlerEx(mySettings->getServiceName(), GlobalHandlerEx, handlers);
  if (myStatusHandle == NULL) {
    LOG.LogErrorFormat(L"Failed to RegisterServiceCtrlHandlerEx. %s", LOG.GetLastError());    
    return;
  }

  ServiceStatus status(myStatusHandle, handlers->GetSupportedControlEventsMask());
  ServiceExecuteProcessTask task(context);

  context->SetServiceSettings(mySettings);
  context->SetServiceTask(&task);
  context->SetServiceStatus(&status);

  if (!context->IsValid()) {
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



