#include "StdAfx.h"
#include "ServiceMain.h"
#include "Logger.h"

const Logger LOG(L"ServiceMain");

ServiceMain::ServiceMain(const RunServiceSettings* settings) 
  : mySettings(settings)
  , myStatusHandle(NULL)
{
}

ServiceMain::~ServiceMain() {
}


DWORD WINAPI GlobalHandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
  return ((ServiceMain*)lpContext)->HandlerEx(dwControl, dwEventType, lpEventData);
}


void ServiceMain::JetServiceMain(const Argz* serviceArgz) {
  LOG.LogDebug(L"Executing JetServiceMain");

  myStatusHandle = RegisterServiceCtrlHandlerEx(mySettings->getServiceName(), GlobalHandlerEx, this);
  if (myStatusHandle == NULL) {
    LOG.LogErrorFormat(L"Failed to RegisterServiceCtrlHandlerEx. %s", LOG.GetLastError());    
    return;
  }
  LOG.LogDebug(L"RegisterServiceCtrlHandler completed");

  ReportSvcStatus(SERVICE_START, NO_ERROR);

  myStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (myStopEvent == NULL) {
    LOG.LogErrorFormat(L"Failed to CreateEvent. %s", LOG.GetLastError());
    ReportSvcStatus(SERVICE_STOP, GetLastError());
    return;
  }

  while(1) {
    // Check whether to stop the service.
    WaitForSingleObject(myStopEvent, INFINITE);
    LOG.LogDebug(L"Stop event processes");

    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR);
    return;
  }
}


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
    LOG.LogInfo(L"SERVICE_CONTROL_TIMECHANGE");
    ///Notifies a service that it should stop
    ///If a service accepts this control code, it must stop upon receipt and return NO_ERROR. 
    ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR);

    // Signal the service to stop.
    SetEvent(myStopEvent);
    return NO_ERROR;

  default:
    return ERROR_CALL_NOT_IMPLEMENTED;
   }   
}

void ServiceMain::ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint ) {
    static DWORD dwCheckPoint = 1;

    SERVICE_STATUS myStatus;

    // Fill in the SERVICE_STATUS structure.
    myStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    myStatus.dwServiceSpecificExitCode = 0;
    myStatus.dwCurrentState = dwCurrentState;
    myStatus.dwWin32ExitCode = dwWin32ExitCode;
    myStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING) {
        myStatus.dwControlsAccepted = 0;
    } else {
      myStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED)) {
        myStatus.dwCheckPoint = 0;
    } else {
      myStatus.dwCheckPoint = dwCheckPoint++;
    }

    // Report the status of the service to the SCM.
    if (0 == SetServiceStatus( myStatusHandle, &myStatus)) {
      LOG.LogWarnFormat(L"Failed to set service status: %s", LOG.GetLastError());
    }
}

