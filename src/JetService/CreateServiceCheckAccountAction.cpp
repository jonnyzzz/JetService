#include "StdAfx.h"
#include "CreateServiceCheckAccountAction.h"
#include "LogonUserCommand.h"
#include "LSAGrantPrivilegeCommand.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceCheckAccountAction");

CreateServiceCheckAccountAction::CreateServiceCheckAccountAction(const CString& name)
  : CreateServiceSettingsAction(name)
{
}


CreateServiceCheckAccountAction::~CreateServiceCheckAccountAction(void)
{
}


class CheckUserAccount : public LogonUserCommand {
public:
  CheckUserAccount(const CreateServiceSettings* settings) : LogonUserCommand(settings, LogonUserMode::AS_SERVICE) {}
  virtual ~CheckUserAccount() {}
public:
  virtual int executeCommand(HANDLE userToken) {
    LOG.LogInfo(L"User logged in as service");
    return 0;
  }
};


int CreateServiceCheckAccountAction:: ExecuteAction(const Argz* az, const CreateServiceSettings* settings, const ServiceTaskSettings* task) {
  if (settings->runAsSystem()) return 0;
  if (!az->GetBooleanArgument(L"checkUserAccount", true)) return 0;
  
  CheckUserAccount check(settings);
  int ret = static_cast<Command*>(&check)->executeCommand();
  if (ret == 0) return 0;

  //TODO: fix me
  if (az->GetBooleanArgument(L"giveUserRights", false)) {
    return TryAddExecuteAsService(az, settings);    
  } else {
    LOG.LogInfo(L"To disable user account check add '/checkUserAccount=false' commandline argument"); 
    return ret;
  }  
}



int CreateServiceCheckAccountAction::TryAddExecuteAsService(const Argz* argz, const CreateServiceSettings* settings) {
  LSAGrantPrivilegeCommand cmd(settings);
  int ret = static_cast<Command*>(&cmd)->executeCommand();
  if (ret == 0) {        
    CheckUserAccount check(settings);
    ret = static_cast<Command*>(&check)->executeCommand();
  }
  if (ret == 0) return 0;

  LOG.LogInfo(L"Failed to give user enough rights to run as server");
  LOG.LogInfo(L"Give user enough rights or add '/giveUserRights=false' commandline argument");
  return 1;
}

