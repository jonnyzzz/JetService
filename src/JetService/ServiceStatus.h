#pragma once

#include "StatusValue.h"


class ServiceStatus
{
public:
  ServiceStatus(SERVICE_STATUS_HANDLE handle);
  virtual ~ServiceStatus(void);

private:
  ServiceStatus(const ServiceStatus& status);

public:
  void SetStatus(const StatusValue* newStatus);

public:
  DWORD ThreadMain();

private:
  static bool UpdateStatus(SERVICE_STATUS* status, const StatusValue* value);

private:
  SERVICE_STATUS_HANDLE myStatusHandle;
  const StatusValue* myCurrentStatus;

  HANDLE myThreadHandle;
  DWORD myThreadId;

  HANDLE myUpdateStatusEvent;
};

