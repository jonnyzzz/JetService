#pragma once

#include "ServiceTask.h"
#include "InterruptHolder.h"


class ThreadedServiceTask : public ServiceTask, public InterruptHolder
{
public:
  ThreadedServiceTask(const ServiceEventContext* context);
  virtual ~ThreadedServiceTask();

public:
   virtual void CallStartService();
   virtual void CallStopSerive();

public:
  DWORD ThreadProcess();

public:
  ///This method will be called from new thread. 
  ///Implementation whould take care of IsInterrupted() 
  ///As the process is started, it should report RUNNING status
  ///This method should perform polling while service is running.
  ///Service will be stopped as method is finished.
  virtual void ExecuteProcess() = 0;

private:
  void ProcessServiceStart();
  void ProcessServiceStop();

private:
  DWORD myProcessThreadId;
  HANDLE myProcessThread;

  CRITICAL_SECTION myLock;
};

