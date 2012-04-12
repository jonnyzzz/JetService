#pragma once

#include "SettingsServiceTask.h"
#include "SimpleThreadedServiceTask.h"

class ServiceExecuteProcessTask : public SettingsServiceTask
{
public:
  ServiceExecuteProcessTask(const ServiceEventContext* context);
  virtual ~ServiceExecuteProcessTask();


public:  
  virtual void ExecuteProcess(const ServiceTaskSettings* settings);

private:
  HANDLE CreateProcessToken();
  STARTUPINFO CreateProcessStartupInfo();
};

