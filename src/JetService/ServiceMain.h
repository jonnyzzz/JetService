#pragma once

#include "ServiceSettings.h"
#include "Argz.h"
#include "ServiceStatus.h"

class ServiceMain {
public:
  ServiceMain(const ServiceSettings* settings);
  virtual ~ServiceMain();

public:
  void JetServiceMain(const Argz* serviceArgz);

private:
  const ServiceSettings* const mySettings;  
};

