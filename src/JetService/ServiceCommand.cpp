#include "StdAfx.h"
#include "Logger.h"
#include "ServiceCommand.h"

const SC_Right SC_Right::ALL_ACCESS = SC_Right(L"All access", SCM_Right::ALL_ACCESS, SERVICE_ALL_ACCESS);
//TODO: weaker permission to remove service?
const SC_Right SC_Right::SERVICE_DELETE = SC_Right(L"Delete", SCM_Right::ENUMERATE_SERVICE, DELETE);
//const SC_Right SC_Right::CHANGE_CONFIG = SC_Right(L"Change config", SERVICE_CHANGE_CONFIG);
//const SC_Right SC_Right::INTERROGATE = SC_Right(L"Interrogate", SERVICE_INTERROGATE);
//const SC_Right SC_Right::QUERY_CONFIG = SC_Right(L"Query config", SERVICE_QUERY_CONFIG);
//const SC_Right SC_Right::START = SC_Right(L"Start", SERVICE_START);
//const SC_Right SC_Right::STOP = SC_Right(L"Stop", SERVICE_STOP);
//const SC_Right SC_Right::USER_DEFINED = SC_Right(L"User defined", SERVICE_USER_DEFINED_CONTROL);

const Logger LOG(L"ServiceCommand");

ServiceCommand::ServiceCommand(const ServiceSettings* settings, const SC_Right& sRight) 
  : mySettings(settings)
  , mySCRight(sRight)
  , ServiceControlManagerCommand(sRight)
{
}


ServiceCommand::~ServiceCommand(void)
{
}

int ServiceCommand::executeCommand(SC_HANDLE scm) {
  LOG.LogDebugFormat(L"Opening %s...", mySettings->getServiceName());

  SC_HANDLE svc = OpenService(scm, mySettings->getServiceName(), mySCRight);

  if (svc == NULL) {
    DWORD err = GetLastError();
    CString msg = CreateFormatted(L"Failed to open service %s.", mySettings->getServiceName());
    switch(err) {
    case ERROR_ACCESS_DENIED:
      LOG.LogWarnFormat(L"%s Access denied", msg);
      return 1;
    
    case ERROR_INVALID_NAME:
      LOG.LogWarnFormat(L"%s Invalid service name", msg);
      return 1;

    case ERROR_SERVICE_DOES_NOT_EXIST:
      LOG.LogWarnFormat(L"%s Service does not exist", msg);
      return 1;
      break;
    case ERROR_INVALID_HANDLE:
    default:
      LOG.LogWarnFormat(L"%s. %s", msg, LOG.GetErrorText(err));
      return 1;
      break;
    }
  }

  int ret = executeCommand(scm, svc);

  CloseServiceHandle(svc);
  return ret;
}

