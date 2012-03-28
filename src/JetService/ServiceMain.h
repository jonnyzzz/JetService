#pragma once

#include "ServiceSettings.h"
#include "Argz.h"
#include "ServiceStatus.h"

class ServiceMain {
public:
  ServiceMain(const RunServiceSettings* settings);
  virtual ~ServiceMain();

public:
  void JetServiceMain(const Argz* serviceArgz);

public:
  DWORD HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData);

private:
  const RunServiceSettings* const mySettings;  
  HANDLE myStopEvent;
};

