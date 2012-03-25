#include "StdAfx.h"
#include "DeleteServiceCommand.h"
#include "Logger.h"

const Logger LOG(L"DeleteServiceCommand");

DeleteServiceCommand::DeleteServiceCommand(const ServiceSettings* settings) 
  : ServiceCommand(settings, SC_Right::SERVICE_DELETE)
{
}


DeleteServiceCommand::~DeleteServiceCommand(void)
{
}


int DeleteServiceCommand::executeCommand(SC_HANDLE scm, SC_HANDLE svc) {
  LOG.LogDebugFormat(L"Removing %s service", mySettings->getServiceName());
  
  if (!DeleteService(svc)) {
    DWORD err = GetLastError();
    CString msg = CreateFormatted(L"Failed to delete service %s.", mySettings->getServiceName());

    if (err == ERROR_SERVICE_MARKED_FOR_DELETE) {
      LOG.LogWarnFormat(L"%s Service is marted for delete.", msg);
      return 1;
    } else {
      LOG.LogWarnFormat(L"%s %s", LOG.GetErrorText(err));
      return 1;
    }
  }

  LOG.LogInfoFormat(L"Service %s will be removed");
  return 0;
}

