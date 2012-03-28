#include "StdAfx.h"
#include "ServiceStatus.h"
#include "Logger.h"

const Logger LOG(L"ServiceStatus");


DWORD WINAPI ServiceStatusThreadMain(LPVOID lpParameter) {
  return ((ServiceStatus*)lpParameter)->ThreadMain();
}

ServiceStatus::ServiceStatus(SERVICE_STATUS_HANDLE handle)
  : myStatusHandle(handle)
  , myCurrentStatus(NULL)
  , myThreadHandle(NULL)
  , myUpdateStatusEvent(NULL)
  , myThreadId(0)
{
  myUpdateStatusEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  if (myUpdateStatusEvent == NULL) {
    LOG.LogErrorFormat(L"Failed to create ManualResetEvent. %s", LOG.GetLastError());
    return;
  }
  
  myThreadHandle = CreateThread(
    NULL, 
    0,
    ServiceStatusThreadMain,
    this, 
    0, 
    &myThreadId);

  if (myThreadHandle == NULL) {
    LOG.LogErrorFormat(L"Failed to create ServiceStatusThread. %s", LOG.GetLastError());
    myThreadId = 0;
  }
}

ServiceStatus::~ServiceStatus(void)
{
  if (myThreadHandle != NULL) {
    //TODO: teminate thread
    if (WAIT_OBJECT_0 != WaitForSingleObject(myThreadHandle, 5000)) {
      LOG.LogWarn(L"Failed to wait for status thread to exit");
    }
    CloseHandle(myThreadHandle);
  }
}


void ServiceStatus::SetStatus(const StatusValue* newStatus) {
  myCurrentStatus = newStatus;
  LOG.LogDebugFormat(L"Set status to: %s", newStatus->GetName());
  SetEvent(myUpdateStatusEvent);
}

bool ServiceStatus::UpdateStatus(SERVICE_STATUS* status, const StatusValue* value) {
  status->dwCheckPoint++;
  if (value == NULL) return false;

  status->dwCurrentState = value->GetState();
  status->dwWaitHint = value->GetWaitHint();
  //TODO: handle error statuses
  status->dwWin32ExitCode = NO_ERROR;
  status->dwServiceSpecificExitCode = 0;
  return true;
}

DWORD ServiceStatus::ThreadMain() {
  const Logger LOG(L"ServiceStatus::Thread");
  
  //see http://msdn.microsoft.com/en-us/library/windows/desktop/ms685996(v=vs.85).aspx
  SERVICE_STATUS status;
  ZeroMemory(&status, sizeof(status));

  status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;  //service type. Could be reused with service registration  
  status.dwControlsAccepted = SERVICE_CONTROL_INTERROGATE | SERVICE_ACCEPT_STOP; //TODO: must be the same as in HandlerEx implementation => introduce class for it.
  status.dwWin32ExitCode = NO_ERROR; //or ERROR_SERVICE_SPECIFIC_ERROR + dwServiceSpecificExitCode
  status.dwServiceSpecificExitCode = 0;
  status.dwCheckPoint = 42;

  DWORD waitTimeSpan = INFINITE;
  while(true) {    
    WaitForSingleObject(myUpdateStatusEvent, waitTimeSpan);
    const StatusValue* nextValue = myCurrentStatus;    

    UpdateStatus(&status, nextValue);
    LOG.LogInfoFormat(L"Call SetSerciceStatus for: %s", nextValue->GetName());    
    if (0 == SetServiceStatus(myStatusHandle, &status)) {
      LOG.LogWarnFormat(L"Failed to set service status: %s", LOG.GetLastError());
    }

    if (nextValue->IsFinalEvent()) {
      LOG.LogDebug(L"Recieved final event. Thread exiting");
      return 0;
    }

    waitTimeSpan = nextValue->IsRepeating() ? nextValue->GetWaitHint() / 3 : INFINITE;
  }


  return 0;
}