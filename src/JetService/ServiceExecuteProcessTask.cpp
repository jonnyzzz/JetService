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
  ProcessCommand cmd(settings);
  int ret = cmd.executeCommand();
  if (ret != 0) {
    LOG.LogError(L"Failed to start process");
    //stopped will be called
  }
}

