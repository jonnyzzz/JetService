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

private:
  const RunServiceSettings* const mySettings;  
};

