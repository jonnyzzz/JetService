#include "StdAfx.h"
#include "LSAPolicyCommand.h"
#include "Logger.h"

const Logger LOG(L"LSAPolicyCommand");

LSAPolicyCommand::LSAPolicyCommand(const LSAPolicyRight& right)
  : myRight(right)
{
}


LSAPolicyCommand::~LSAPolicyCommand()
{
}


int LSAPolicyCommand::executeCommand() {
  LOG.LogDebugFormat(L"Opening LSAPolicy with %s right", (CString)myRight);

  LSA_HANDLE handle;
  NTSTATUS status = LsaOpenPolicy(NULL, NULL, myRight, &handle);
  if (status != ERROR_SUCCESS) {
    CString message = CreateFormatted(L"Failed to open LSAPolicy with %s right.", (CString)myRight);
    if (status == ERROR_ACCESS_DENIED) {
      LOG.LogErrorFormat(L"%s Access denied", message);
    } else {      
      LOG.LogErrorFormat(L"%s. Error code %ld", message, status);
    }
    return 1;
  }

  int ret = executeCommand(handle);
  
  LsaClose(handle);
  return ret;
}
