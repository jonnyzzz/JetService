#include "StdAfx.h"
#include "ServiceExecuteProcessTask.h"
#include "ServiceEventContext.h"
#include "ServiceStatus.h"
#include "ServiceProcessCommand.h"
#include "Logger.h"

const Logger LOG(L"ServiceExecuteProcessTask");

ServiceExecuteProcessTask::ServiceExecuteProcessTask(const ServiceEventContext* context)
  : SettingsServiceTask(context)
{
}


ServiceExecuteProcessTask::~ServiceExecuteProcessTask()
{
}

void ServiceExecuteProcessTask::ExecuteProcess(const ServiceTaskSettings* settings) {
  //Hack: we should return running as preparations completed, not at this moment.
  //Hack: ProcessCommand api is neede for it.
  myContext->GetServiceStatus()->SetStatus(StatusValue::RUNNING);
  
  ServiceProcessCommand cmd(settings, this);
  int ret = static_cast<Command*>(&cmd)->executeCommand();
  if (ret != 0) {
    LOG.LogErrorFormat(L"Failed to start process: %d", ret);
    //stopped will be called
  }
}

