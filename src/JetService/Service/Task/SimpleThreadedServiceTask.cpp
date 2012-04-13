#include "StdAfx.h"
#include "SimpleThreadedServiceTask.h"
#include "ServiceEventContext.h"
#include "ServiceStatus.h"
#include "Logger.h"

const Logger LOG(L"SimpleThreadedServiceTask");

SimpleThreadedServiceTask::SimpleThreadedServiceTask(const ServiceEventContext* context)
  : ThreadedServiceTask(context)
{
}


SimpleThreadedServiceTask::~SimpleThreadedServiceTask(void)
{
}


void SimpleThreadedServiceTask::ExecuteProcess() {
  //report service is running
  myContext->GetServiceStatus()->SetStatus(StatusValue::RUNNING);
  while(true) {
    Sleep(500);
    if (IsInterrupted()) {
      LOG.LogDebug(L"Interrupted flag is detected");
      break;
    }
  }
}

