#pragma once

#include "ServiceTask.h"
#include "InterruptHolder.h"


class ThreadedServiceTask : public ServiceTask, protected InterruptHolder
{
public:
  ThreadedServiceTask(const ServiceEventContext* context);
  virtual ~ThreadedServiceTask();

public:
   virtual void CallStartService();
   virtual void CallStopSerive();

public:
  DWORD ThreadProcess();

private:
  void ProcessServiceStart();
  void ProcessServiceStop();

private:
  DWORD myProcessThreadId;
  HANDLE myProcessThread;

  CRITICAL_SECTION myLock;


};

