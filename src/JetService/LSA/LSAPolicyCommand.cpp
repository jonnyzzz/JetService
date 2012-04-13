#include "StdAfx.h"
#include "LSAPolicyCommand.h"
#include "Logger.h"

const Logger LOG(L"LSAPolicyCommand");

const LSAPolicyRight LSAPolicyRight::CHANGE_PRIVILEGES = LSAPolicyRight(L"Change privileges", POLICY_ALL_ACCESS);
const LSAPolicyRight LSAPolicyRight::READ_PRIVILEGES = LSAPolicyRight(L"Read privileges", POLICY_READ);

LSAPolicyCommand::LSAPolicyCommand(const LSAPolicyRight& right)
  : myRight(right)
{
}


LSAPolicyCommand::~LSAPolicyCommand()
{
}


int LSAPolicyCommand::executeCommand() {
  LOG.LogDebugFormat(L"Opening LSAPolicy with %s right", (CString)myRight);

  LSA_OBJECT_ATTRIBUTES attrs;
  ZeroMemory(&attrs, sizeof(attrs));
  attrs.Length = sizeof(attrs);

  LSA_HANDLE handle;  
  NTSTATUS status = LsaOpenPolicy(NULL, &attrs, myRight, &handle);
  
  LOG.LogDebug(L"LsaOpenPolicy called");
  if (status != ERROR_SUCCESS) {
    CString message = CreateFormatted(L"Failed to open LSAPolicy with %s right.", (CString)myRight);
    if (status == ERROR_ACCESS_DENIED) {
      LOG.LogErrorFormat(L"%s Access denied", message);
    } else {      
      LOG.LogErrorFormat(L"%s. Error code %ld", message, status);
    }
    return 1;
  }

  LOG.LogDebug(L"Opened LSA policy");
  int ret = executeCommand(handle);
  
  LsaClose(handle);
  return ret;
}
