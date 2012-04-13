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

