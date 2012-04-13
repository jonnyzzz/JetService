#include "StdAfx.h"
#include "ValidateLogonSIDAction.h"
#include "LSAUserCommand.h"
#include "Logger.h"
#include "Sddl.h"

const Logger LOG(L"ValidateServiceTaskAction");

ValidateLogonSIDAction::ValidateLogonSIDAction(void)
  : CreateServiceSettingsAction(L"validate-sid")
{
}


ValidateLogonSIDAction::~ValidateLogonSIDAction(void)
{
}


void ValidateLogonSIDAction::PrintUsage(ConsoleWriter* writer) {
  //NOP as the command in hidden
}

class DumpSIDCommand : public LSAUserCommand {
public:
  DumpSIDCommand(const CreateServiceSettings* settings) : LSAUserCommand(settings, LSAPolicyRight::READ_PRIVILEGES) {}
  virtual ~DumpSIDCommand() {}
public:
  virtual int executeCommand(LSA_HANDLE lsa, PSID sid) {
    LPTSTR sSid;
    if (0 == ConvertSidToStringSid(sid, &sSid)) {
      LOG.LogErrorFormat(L"Failed to convert SID to string. %s", LOG.GetLastError());
      return 1;
    }
    LOG.LogInfoFormat(L"User token opened: SID=%s", sSid);
    LocalFree(sSid);
    return 0;
  }
};

int ValidateLogonSIDAction::ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* taskSettings) {
  DumpSIDCommand cmd(settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}
