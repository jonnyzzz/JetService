#pragma once

#include "ServiceSettings.h"

class ServiceMain {
public:
  ServiceMain(const RunServiceSettings* settings);
  virtual ~ServiceMain();

public:
  void JetServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);

private:
  const RunServiceSettings* const mySettings;
};

