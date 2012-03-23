#include "StdAfx.h"
#include "Logger.h"
#include "ServiceControlManagerCommand.h"

const SCM_Right SCM_Right::ALL_ACCESS = SCM_Right(L"All access", SC_MANAGER_ALL_ACCESS);
const SCM_Right SCM_Right::CREATE_SERVICE = SCM_Right(L"Create service", SC_MANAGER_CREATE_SERVICE);
const SCM_Right SCM_Right::ENUMERATE_SERVICE = SCM_Right(L"Enumerate service", SC_MANAGER_ENUMERATE_SERVICE);

Logger LOG(L"ServiceControlManagerCommand");

int ServiceControlManagerCommand::executeCommand() {
  LOG.LogDebug(CString(L"Openning ServiceControlManager with ") + myRight + L" permissions");

  SC_HANDLE handle = OpenSCManager(NULL, NULL, myRight);
  if (handle == NULL) {
    DWORD error = GetLastError();

    CString err;
    err.Format(L"Failed to open Service Control Manager: " + LOG.GetLastError());
    return 1;
  }

  int res = this->executeCommand(handle);

  CloseServiceHandle(handle);
  return res;
}