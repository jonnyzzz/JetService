#include "StdAfx.h"
#include "ServiceExecuteProcessTask.h"
#include "ServiceEventContext.h"
#include "ServiceStatus.h"
#include "ProcessCommand.h"
#include "Logger.h"

const Logger LOG(L"ServiceExecuteProcessTask");

ServiceExecuteProcessTask::ServiceExecuteProcessTask(const ServiceEventContext* context)
  : SettingsServiceTask(context)
{
}


ServiceExecuteProcessTask::~ServiceExecuteProcessTask()
{
}

void ServiceExecuteProcessTask::ExecuteProcess() {
  //make explicit
  SettingsServiceTask::ExecuteProcess();
}

void ServiceExecuteProcessTask::ExecuteProcess(const ServiceTaskSettings* settings) {
  //Hack: we should return running as preparations completed, not at this moment.
  //Hack: ProcessCommand api is neede for it.
  myContext->GetServiceStatus()->SetStatus(StatusValue::RUNNING);

  ProcessCommand cmd(settings, this);
  int ret = cmd.executeCommand();
  if (ret != 0) {
    LOG.LogError(L"Failed to start process");
    //stopped will be called
  }
}

