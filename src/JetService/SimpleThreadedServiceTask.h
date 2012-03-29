#pragma once

#include "ThreadedServiceTask.h"

class SimpleThreadedServiceTask : public ThreadedServiceTask
{
public:
  SimpleThreadedServiceTask(const ServiceEventContext* context);
  virtual ~SimpleThreadedServiceTask();

public:
  virtual void ExecuteProcess();
};

