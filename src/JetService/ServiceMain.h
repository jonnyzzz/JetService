#pragma once

#include "ServiceSettings.h"
#include "Argz.h"

class ServiceMain {
public:
  ServiceMain(const RunServiceSettings* settings);
  virtual ~ServiceMain();

public:
  void JetServiceMain(const Argz* serviceArgz);

public:
  DWORD HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);

private:
  void ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint = 0);

private:
  const RunServiceSettings* const mySettings;
  SERVICE_STATUS_HANDLE myStatusHandle;  
  HANDLE myStopEvent;
};

