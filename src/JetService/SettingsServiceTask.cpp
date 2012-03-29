#include "StdAfx.h"
#include "SettingsServiceTask.h"
#include "FileTaskSettings.h"
#include "ServiceStatus.h"
#include "ServiceEventContext.h"
#include "Logger.h"

const Logger LOG(L"SettingsServiceTask");

SettingsServiceTask::SettingsServiceTask(const ServiceEventContext* context)
  : ThreadedServiceTask(context)
{
}


SettingsServiceTask::~SettingsServiceTask(void)
{
}


class SettingsLoader : public FileTaskSettings {
public:
  SettingsLoader(const RunServiceSettings* runSettings, SettingsServiceTask *task) 
    : FileTaskSettings(runSettings)
   , myTask(task) {
  }

  virtual ~SettingsLoader() {}

private:
  SettingsServiceTask* myTask;

public:
  virtual int executeCommand(const ServiceTaskSettings* settings) {
    myTask->ExecuteProcess(settings);
    return 0;
  }
};


void SettingsServiceTask::ExecuteProcess() {
  SettingsLoader loader(myContext->GetServiceSettings(), this);

  int ret = static_cast<Command*>(&loader)->executeCommand();
  if (0 != ret) {
    LOG.LogError(L"Failed to load service task settings. Service will exit");
    myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
    return;
  }  
}


