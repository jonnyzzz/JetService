#include "StdAfx.h"
#include "ValidateCreateServiceAction.h"
#include "Logger.h"

const Logger LOG(L"ValidateCreateServiceAction");

ValidateCreateServiceAction::ValidateCreateServiceAction()
  : CreateServiceSettingsAction(L"validate-create-settings")
{
}


ValidateCreateServiceAction::~ValidateCreateServiceAction()
{
}


void ValidateCreateServiceAction::PrintUsage(ConsoleWriter* writer) {
  //NOP as the command in hidden
}

int ValidateCreateServiceAction::ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* taskSettings) {
  LOG.LogInfoFormat(L"[j]: ServiceName:%s!~", settings->getServiceName());
  LOG.LogInfoFormat(L"[j]: ServiceDisplayName:%s!~", settings->getServiceDisplayName());
  LOG.LogInfoFormat(L"[j]: ServiceDescription:%s!~", settings->getServiceDescription());
  LOG.LogInfoFormat(L"[j]: IsAutostart:%s!~", settings->isAutostart() ? L"true": L"false" );
  LOG.LogInfoFormat(L"[j]: RunAsService:%s!~", settings->runAsSystem() ? L"true": L"false" );
  if (!settings->runAsSystem()) {
    LOG.LogInfoFormat(L"[j]: UserName:%s!~", settings->getUserName());
    LOG.LogInfoFormat(L"[j]: Domain:%s!~", settings->getDomain());
    LOG.LogInfoFormat(L"[j]: Password:%s!~", settings->getPassword());
  }
  LOG.LogInfoFormat(L"[e]: Program:%s!~", taskSettings->getProgramPath());
  LOG.LogInfoFormat(L"[e]: Arguments:%s!~", taskSettings->getProgramArguments());
  LOG.LogInfoFormat(L"[e]: WorkDir:%s!~", taskSettings->getWorkDir());
  return 0;
}
