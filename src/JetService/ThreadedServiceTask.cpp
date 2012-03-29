#include "StdAfx.h"
#include "ThreadedServiceTask.h"
#include "ServiceEventContext.h"
#include "ServiceStatus.h"
#include "Logger.h"

const Logger LOG(L"ThreadedServiceTask");

ThreadedServiceTask::ThreadedServiceTask(const ServiceEventContext* context)
  : ServiceTask(context)
  , myProcessThread(NULL)
  , myProcessThreadId(0)
{
  InitializeCriticalSection(&myLock);
}


ThreadedServiceTask::~ThreadedServiceTask(void)
{
  DeleteCriticalSection(&myLock);
  if (myProcessThread != NULL) {
    LOG.LogDebug(L"Process thread is not finished!");
    Interrupt();
    WaitForSingleObject(myProcessThread, INFINITE);
  }
}

DWORD WINAPI ThreadedServiceTaskThreadProc(LPVOID lpParameter) {
  return ((ThreadedServiceTask*)lpParameter)->ThreadProcess();
}

void ThreadedServiceTask::CallStartService() {
  LOG.LogInfo(L"CallStartService");
  myContext->GetServiceStatus()->SetStatus(StatusValue::STARTING);

  EnterCriticalSection(&myLock);
  ProcessServiceStart();
  LeaveCriticalSection(&myLock);
}

void ThreadedServiceTask::ProcessServiceStart() {
  if (myProcessThread != NULL) {
    //TODO: process this cause
    LOG.LogWarn(L"There is running process thread. Will terminate");
    myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);    
    return;
  }

  myProcessThread = CreateThread(NULL, 0, &ThreadedServiceTaskThreadProc, this, 0, &myProcessThreadId);
  if (myProcessThread == NULL) {
    LOG.LogError(L"Failed to create thread to execute service task");
    myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
    return;
  }
  LOG.LogDebug(L"Started service task thread");
}

void ThreadedServiceTask::CallStopSerive() {
  LOG.LogInfo(L"CallStopService");
  myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPING);
  
  EnterCriticalSection(&myLock);
  ProcessServiceStop();
  LeaveCriticalSection(&myLock);
}

void ThreadedServiceTask::ProcessServiceStop() {
  if (myProcessThread == NULL) {
    LOG.LogWarn(L"No service process thread is running. Will report stopped");
    myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
    return;
  }

  Interrupt();
}

DWORD ThreadedServiceTask::ThreadProcess() {
  const Logger LOG(L"ThreadedServiceTask:Thread");
  LOG.LogDebug(L"Thread started");
  
  //report service is running
  myContext->GetServiceStatus()->SetStatus(StatusValue::RUNNING);
  while(true) {
    Sleep(500);
    if (IsInterrupted()) {
      LOG.LogDebug(L"Interrupted flag is detected");
      break;
    }
  }

  LOG.LogDebugFormat(L"Task finished. Interrupted=%s.", IsInterrupted() ? L"true" : L"false");
  myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
  myProcessThread = NULL;
  myProcessThreadId = 0;
  return 0;
}


