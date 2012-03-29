#include "StdAfx.h"
#include "ServiceTask.h"
#include "ServiceEventContext.h"
#include "ServiceStatus.h"
#include "Logger.h"

const Logger LOG(L"ServiceTask");

ServiceTask::ServiceTask(const ServiceEventContext* context)
  : myContext(context)
{
}

ServiceTask::~ServiceTask()
{
}


void ServiceTask::CallStartService() {
  LOG.LogInfo(L"CallStartService");
  myContext->GetServiceStatus()->SetStatus(StatusValue::RUNNING);
}


void ServiceTask::CallStopSerive() {
  LOG.LogInfo(L"CallStopService");
  myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
}
