#pragma once

#include "ServiceStatus.h"

class ServiceEventContext
{
public:
  ServiceEventContext();
  virtual ~ServiceEventContext();

public:

  virtual ServiceStatus* GetServiceStatus() const = 0;
  bool IsValid() const;

};


class ServiceEventContextEx : public ServiceEventContext {
public: 
  ServiceEventContextEx();
  virtual ~ServiceEventContextEx();

public:
  virtual ServiceStatus* GetServiceStatus() const;
  void SetServiceStatus(ServiceStatus* status);

private:
  ServiceStatus* myServiceStatus;

};



