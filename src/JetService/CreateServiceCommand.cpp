#include "StdAfx.h"
#include "CreateServiceCommand.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceCommand");

CreateServiceCommand::CreateServiceCommand(const CreateServiceSettings* settings) 
  : mySettings(settings), 
  ServiceControlManagerCommand(SCM_Right::CREATE_SERVICE)
{
}

CreateServiceCommand::~CreateServiceCommand()
{
}

int CreateServiceCommand::executeCommand(SC_HANDLE scm) {
  LOG.LogDebugFormat(L"Creating and registering service: name=%s...", mySettings->getServiceName());
  
  LPCWSTR userName = NULL;
  LPCWSTR password = NULL;

  if (!mySettings->runAsSystem()) {
    userName = mySettings->getUserName();
    password = mySettings->getPassword();
  }

  SC_HANDLE service = CreateService(
    scm, //ServiceControlManager instance
    mySettings->getServiceName(),
    mySettings->getServiceDisplayName(),
    SERVICE_ALL_ACCESS, //any option here?
    SERVICE_WIN32_OWN_PROCESS, //Consider to have SERVICE_INTERACTIVE_PROCESS
    mySettings->isAutostart() ? SERVICE_AUTO_START : SERVICE_DEMAND_START, 
    SERVICE_ERROR_NORMAL, 
    NULL, //TODO: executable to be started from service
    NULL, //Start orger group
    NULL, //tag
    NULL, //dependencies
    userName,
    password
  );

  if (service == NULL) {
    DWORD err = GetLastError();
    LOG.LogErrorFormat(L"Failed to create service. %s", LOG.GetLastError());
    return 1;
  }

  LOG.LogInfoFormat(L"Service %s was created", mySettings->getServiceName());
  CloseServiceHandle(service);

  return 0;
}