#include "StdAfx.h"
#include "Logger.h"
#include "ServiceControlManagerCommand.h"

const SCM_Right SCM_Right::ALL_ACCESS = SCM_Right(L"All access", SC_MANAGER_ALL_ACCESS);
const SCM_Right SCM_Right::CREATE_SERVICE = SCM_Right(L"Create service", SC_MANAGER_CREATE_SERVICE);
const SCM_Right SCM_Right::ENUMERATE_SERVICE = SCM_Right(L"Enumerate service", SC_MANAGER_ENUMERATE_SERVICE);

Logger LOG(L"ServiceControlManagerCommand");

int ServiceControlManagerCommand::executeCommand() {
  LOG.LogDebugFormat(L"Openning ServiceControlManager with %s permissions", (CString)myRight);

  SC_HANDLE handle = OpenSCManager(NULL, NULL, myRight);
  if (handle == NULL) {
    DWORD err = GetLastError();
    CString msg = CreateFormatted(L"Failed to open Service Control Manager with %s permissions.", (CString)myRight);
    if (err == ERROR_ACCESS_DENIED) {
      LOG.LogErrorFormat(L"%s Not enough rights", msg);
      return 1;
    } 
    LOG.LogErrorFormat(L"%s %s", msg, LOG.GetErrorText(err));
    return 1;
  }

  int res = this->executeCommand(handle);

  CloseServiceHandle(handle);
  return res;
}