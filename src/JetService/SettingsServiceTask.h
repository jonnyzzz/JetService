#pragma once

#include "ThreadedServiceTask.h"
#include "ServiceSettings.h"

class SettingsServiceTask : public ThreadedServiceTask
{
public:
  SettingsServiceTask(const ServiceEventContext* context);
  virtual ~SettingsServiceTask();

public:
  virtual void ExecuteProcess();

public:
  virtual void ExecuteProcess(const ServiceTaskSettings* settings) = 0;
};

