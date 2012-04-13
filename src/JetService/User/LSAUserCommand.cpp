#include "StdAfx.h"
#include "LSAUserCommand.h"
#include "Logger.h"

const Logger LOG(L"LSAUserCommand");

 LSAUserCommand::LSAUserCommand(const CreateServiceSettings* settings, const LSAPolicyRight& lsaRight)
  : LSAPolicyCommand(lsaRight)
  , mySettings(settings)
  , myLSARight(lsaRight)
{
}

LSAUserCommand::~LSAUserCommand()
{
}

int LSAUserCommand::executeCommand(LSA_HANDLE handle) {    
  CString accountName = CreateFormatted(L"%s", mySettings->getUserName());

  const DWORD sz = 2044;
  PSID sid = (PSID)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sz + 1);  
  TCHAR foundDomain[sz+1];
  DWORD psz = sz;
  DWORD pdz = sz;
  SID_NAME_USE use = SidTypeUser;

  LOG.LogDebug(L"Call LookupAccountName");
  if (0 == LookupAccountName(NULL, accountName, sid, &psz, foundDomain, &pdz, &use)) {
    LOG.LogErrorFormat(L"Failed to LookupAccountName. %s", LOG.GetLastError());
    return 1;
  }

  foundDomain[pdz] = L'\0';
  LOG.LogDebugFormat(L"LSA Policy opened: Domain %s", foundDomain);
  return executeCommand(handle, sid);
}


