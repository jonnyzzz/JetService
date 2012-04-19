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
  
  CString sUserName = CreateFormatted(L"%s\\%s", mySettings->getDomain(), mySettings->getUserName());
  CString sPassword = mySettings->getPassword();

  LPCWSTR userName = NULL;
  LPCWSTR password = NULL;

  if (!mySettings->runAsSystem()) {
    //use declared variables to avoid strings from distruct
    userName = sUserName;
    password = sPassword;
    LOG.LogInfoFormat(L"Installing service for user: %s", userName);
  }

  SC_HANDLE service = CreateService(
    scm, //ServiceControlManager instance
    mySettings->getServiceName(),
    mySettings->getServiceDisplayName(),
    SERVICE_ALL_ACCESS, //any option here?
    SERVICE_WIN32_OWN_PROCESS, //Consider to have SERVICE_INTERACTIVE_PROCESS
    mySettings->isAutostart() ? SERVICE_AUTO_START : SERVICE_DEMAND_START, 
    SERVICE_ERROR_NORMAL, 
    mySettings->getServiceExecutable(), //executable command    
    NULL, //Start orger group
    NULL, //tag
    NULL, //dependencies
    userName,
    password
  );

  if (service == NULL) {
    DWORD err = GetLastError();
    CString msg = CreateFormatted(L"Failed to create service %s.", mySettings->getServiceName());
    switch(err) {
    case ERROR_ACCESS_DENIED:
      LOG.LogWarnFormat(L"%s Access denied", msg);
      return 1;
    case ERROR_CIRCULAR_DEPENDENCY:
      LOG.LogWarnFormat(L"%s A circular service dependency was specified", msg);
      return 1;
    case ERROR_DUPLICATE_SERVICE_NAME:
      LOG.LogWarnFormat(L"%s Service display name is already used", msg);
      return 1;
    case ERROR_INVALID_NAME:
      LOG.LogWarnFormat(L"%s The specified service name is invalid", msg);
      return 1;
    case ERROR_INVALID_PARAMETER:
      LOG.LogWarnFormat(L"%s A parameter that was specified is invalid", msg);
      return 1;
    case ERROR_INVALID_SERVICE_ACCOUNT:
      LOG.LogWarnFormat(L"%s Specified account does not exist", msg);
      return 1;
    case ERROR_SERVICE_EXISTS:
      LOG.LogWarnFormat(L"%s The service already exists", msg);
      return 1;
    case ERROR_SERVICE_MARKED_FOR_DELETE:
      LOG.LogWarnFormat(L"%s The service already exists and marked for deletion", msg);
      return 1;
    case ERROR_INVALID_HANDLE:
    default:
      LOG.LogErrorFormat(L"%s %s", msg, LOG.GetLastError());
      return 1;
    }    
  }

  LOG.LogInfoFormat(L"Service '%s' was created", mySettings->getServiceName());
  updateServiceDescription();

  CloseServiceHandle(service);

  return 0;
}


void CreateServiceCommand::updateServiceDescription() {
  //Another possbile approach is to call ChangeServiceConfig2 
  //that is only supported from WinXP/2003
  LOG.LogDebug(L"Updating service description");

  CString path = CreateFormatted(L"SYSTEM\\CurrentControlSet\\Services\\%s", mySettings->getServiceName());
  CRegKey reg(HKEY_LOCAL_MACHINE);
  if (ERROR_SUCCESS != reg.Open(reg, path, KEY_WRITE)) {
    LOG.LogWarnFormat(L"Failed to open registry key %s", path);
    return;
  }

  CString key = L"Description";
  if (ERROR_SUCCESS != reg.SetStringValue(key, mySettings->getServiceDescription())) {
    LOG.LogWarnFormat(L"Failed to set %s for %s", key, path);
    return;
  }

  reg.Flush();
  LOG.LogDebug(L"Service description is updated");
}

