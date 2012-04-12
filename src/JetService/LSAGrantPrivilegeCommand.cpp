#include "StdAfx.h"
#include "LSAGrantPrivilegeCommand.h"
#include "Logger.h"

const Logger LOG(L"LSAGrantPrivilegeCommand");

LSAGrantPrivilegeCommand::LSAGrantPrivilegeCommand(const CreateServiceSettings* settings)
  : LSAUserCommand(settings, LSAPolicyRight::ADD_PRIVILEGE)
{
}


LSAGrantPrivilegeCommand::~LSAGrantPrivilegeCommand()
{
}

void LSAGrantPrivilegeCommand::InitializeUnicodeString(LSA_UNICODE_STRING& x, LPWSTR value) {
  ZeroMemory(&x, sizeof(x));
  x.Buffer = value;
  x.Length = wcslen(x.Buffer) * sizeof(WCHAR);
  x.MaximumLength = (1 + wcslen(x.Buffer)) * sizeof(WCHAR);
}

int LSAGrantPrivilegeCommand::executeCommand(LSA_HANDLE lsa, HANDLE userToken, PSID sid) {
  LOG.LogDebug(L"Start changing user previleges");

  {
    LSA_UNICODE_STRING right;
    InitializeUnicodeString(right, SE_SERVICE_LOGON_NAME);

    NTSTATUS st = LsaAddAccountRights(lsa, sid, &right, 1);
    LOG.LogDebug(L"Called LsaAddAccountRights");

    if (st != ERROR_SUCCESS) {    
      CString message(L"Failed to add account privilege.");
      if (st == ERROR_NO_SUCH_PRIVILEGE) {
        LOG.LogErrorFormat(L"%s No such privelege", message);
      } else if (st == ERROR_ACCESS_DENIED) {
        LOG.LogErrorFormat(L"%s Access denied", message);
      } else {
        LOG.LogErrorFormat(L"%s NTSTATUS=%lx", message, st);      
      }
      return 1;
    }
  }

  LOG.LogDebug(L"Added previleges");

  {
    LSA_UNICODE_STRING right;
    InitializeUnicodeString(right, SE_DENY_SERVICE_LOGON_NAME);

    NTSTATUS st = LsaRemoveAccountRights(lsa, sid, FALSE, &right, 1);

    if (st != ERROR_SUCCESS) {    
      CString message(L"Failed to remove account privilege.");
      if (st == ERROR_NO_SUCH_PRIVILEGE) {
        LOG.LogErrorFormat(L"%s No such privelege", message);
      } else if (st == ERROR_ACCESS_DENIED) {
        LOG.LogErrorFormat(L"%s Access denied", message);
      } else {
        LOG.LogErrorFormat(L"%s NTSTATUS=%lx", message, st);      
      }
      return 1;
    }
  }

  LOG.LogDebug(L"Removed deny previleges");
  return 0;
}

