#include "StdAfx.h"
#include "CreateServiceCheckAccountAction.h"
#include "LogonUserCommand.h"
#include "LSAGrantPrivilegeCommand.h"
#include "CheckLogonAsServiceCommand.h"
#include "CheckAndGrantLogOnAsServiceCommand.h"
#include "Logger.h"


const Logger LOG(L"CreateServiceCheckAccountAction");

CreateServiceCheckAccountAction::CreateServiceCheckAccountAction(const CString& name)
  : CreateServiceSettingsAction(name)
{
}


CreateServiceCheckAccountAction::~CreateServiceCheckAccountAction(void)
{
}


const CString CreateServiceCheckAccountAction::KEY_CHECK_USER_ACCOUNT = L"checkUserAccount";
const CString CreateServiceCheckAccountAction::KEY_GIVE_USER_LOGON = L"giveUserRights";

CString CreateServiceCheckAccountAction::GetUsageString() {
  return CreateFormatted(L"[%s] [%s]", Argz::MakeBoolArgument(KEY_CHECK_USER_ACCOUNT, true), Argz::MakeBoolArgument(KEY_GIVE_USER_LOGON, true));
}

int CreateServiceCheckAccountAction:: ExecuteAction(const Argz* az, const CreateServiceSettings* settings, const ServiceTaskSettings* task) {
  if (settings->runAsSystem()) return 0;
  if (!az->GetBooleanArgument(KEY_CHECK_USER_ACCOUNT, true)) return 0;
  
  CheckLogonAsServiceCommand check(settings);
  int ret = static_cast<Command*>(&check)->executeCommand();  
  if (ret == 0) return 0;

  if (az->GetBooleanArgument(KEY_GIVE_USER_LOGON, true)) {
    return TryAddExecuteAsService(az, settings);    
  } else {
    LOG.LogInfoFormat(L"To disable user account check add '%s' commandline argument", az->MakeBoolArgument(KEY_CHECK_USER_ACCOUNT)); 
    return ret;
  }  
}


int CreateServiceCheckAccountAction::TryAddExecuteAsService(const Argz* az, const CreateServiceSettings* settings) {
  CheckAndGrantLogOnAsServiceCommand cmd(settings, false);
  int ret = static_cast<Command*>(&cmd)->executeCommand();
  if (ret == 0) return 0;

  LOG.LogInfo(L"Failed to give user enough rights to run as server");
  LOG.LogInfoFormat(L"Give user enough rights or add '%s' commandline argument", az->MakeBoolArgument(KEY_GIVE_USER_LOGON));
  return 1;
}

