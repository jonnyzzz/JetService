#include "StdAfx.h"
#include "LSAUserCommand.h"
#include "Logger.h"

const Logger LOG(L"LSAUserCommand");

 LSAUserCommand::LSAUserCommand(const CreateServiceSettings* settings, const LSAPolicyRight& lsaRight)
  : LogonUserSIDCommand(settings)
  , mySettings(settings)
  , myLSARight(lsaRight)
{
}


LSAUserCommand::~LSAUserCommand()
{
}

class LSA : public LSAPolicyCommand {
public:
  LSA(const LSAPolicyRight& right, LSAUserCommand* cmd, HANDLE user, PSID sid, const CreateServiceSettings* settings) 
    : LSAPolicyCommand(right) 
    , mySettings(settings)
    , myCommand(cmd)
    , myUser(user)
    , mySid(sid) { }
  virtual ~LSA() {}
public:
  virtual int executeCommand(LSA_HANDLE handle) {    
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
    return myCommand->executeCommand(handle, myUser, sid);
  }

private:
  const CreateServiceSettings* mySettings;
  LSAUserCommand* myCommand;
  HANDLE myUser;
  PSID mySid;
};


int LSAUserCommand::executeCommand(HANDLE userToken, PSID sid) {  
  LSA cmd(myLSARight, this, userToken, sid, mySettings);
  return static_cast<Command*>(&cmd)->executeCommand();
}

