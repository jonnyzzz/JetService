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
  //NOP as the command in hidden
}


int ValidateServiceTaskAction::ExecuteAction(const Argz* argz, const ServiceTaskSettings* settings) {
  LOG.LogInfoFormat(L"[j]: ServiceName:%s!~", settings->getServiceName());
  LOG.LogInfoFormat(L"[j]: ServiceDisplayName:%s!~", settings->getServiceDisplayName());
  LOG.LogInfoFormat(L"[j]: ServiceDescription:%s!~", settings->getServiceDescription());
  LOG.LogInfoFormat(L"[e]: Program:%s!~", settings->getProgramPath());
  LOG.LogInfoFormat(L"[e]: Arguments:%s!~", settings->getProgramArguments());
  LOG.LogInfoFormat(L"[e]: WorkDir:%s!~", settings->getWorkDir());
  return 0;
}
