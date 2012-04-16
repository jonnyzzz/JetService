#pragma once

#include "ServiceSettings.h"
#include "Argz.h"
#include "ServiceStatus.h"

class ServiceEventContextEx;
class ServiceEventHandlerCollection;


class ServiceMain {
public:
  ServiceMain(const ServiceSettings* settings);
  virtual ~ServiceMain();

public:
  void JetServiceMain(const Argz* serviceArgz);

private:
  void JetServiceMain(const Argz* serviceArgz, ServiceEventContextEx* context, ServiceEventHandlerCollection* handlers);

private:
  const ServiceSettings* const mySettings;  
};

