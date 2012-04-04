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
    LOG.LogDebug(L"LSA Policy opened");
    return myCommand->executeCommand(handle, myUser, mySid);
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

