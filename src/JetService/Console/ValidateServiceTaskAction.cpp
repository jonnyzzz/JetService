#include "StdAfx.h"
#include "ValidateServiceTaskAction.h"
#include "Logger.h"

const Logger LOG(L"ValidateServiceTaskAction");


ValidateServiceTaskAction::ValidateServiceTaskAction()
  : ServiceSettingsAction(L"validate-settings")
{
}


ValidateServiceTaskAction::~ValidateServiceTaskAction()
{
}


void ValidateServiceTaskAction::PrintUsage(ConsoleWriter* writer) {
  if (LOG.IsDebugEnabled()) {
      writer->WriteFormat(L"    %s /%s=<path to settings file>", myName, SettingsKeyName);
      writer->Write      (L"      validates command parse");
      writer->Write();
  }  
}


int ValidateServiceTaskAction::ExecuteAction(const Argz* argz, const ServiceTaskSettings* settings) {
  LOG.LogInfoFormat(L"[j]: ServiceName:%s!~", settings->getServiceName());
  LOG.LogInfoFormat(L"[j]: ServiceDisplayName:%s!~", settings->getServiceDisplayName());
  LOG.LogInfoFormat(L"[j]: ServiceDescription:%s!~", settings->getServiceDescription());
  LOG.LogInfoFormat(L"[e]: Program:%s!~", settings->getProgramPath());
  LOG.LogInfoFormat(L"[e]: Arguments:%s!~", settings->getProgramArguments());
  LOG.LogInfoFormat(L"[e]: WorkDir:%s!~", settings->getWorkDir());
  LOG.LogInfoFormat(L"[e]: StopTimeout:%ld!~", settings->getTerminateWaitTimeoutMilliseconds());

  const ExecutionSettings* exec = settings->getStopCommand();
  if (exec != NULL) {
    LOG.LogInfoFormat(L"[s]: Program:%s!~", exec->getProgramPath());
    LOG.LogInfoFormat(L"[s]: Arguments:%s!~", exec->getProgramArguments());
    LOG.LogInfoFormat(L"[s]: WorkDir:%s!~", exec->getWorkDir());
    LOG.LogInfo(L"[s]: DISABLED:false!~");
  } else {
    LOG.LogInfo(L"[s]: DISABLED:true!~");
  }
  return 0;
}
