#include "StdAfx.h"
#include "CreateServiceAction.h"
#include "CreateServiceCommand.h"
#include "SimpleServiceSettings.h"
#include "ServiceAction.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceAction");


CreateServiceAction::CreateServiceAction(void) : SimpleConsoleAction(L"install")
{
}


CreateServiceAction::~CreateServiceAction(void)
{
}


void CreateServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s /%s=<path to settings file> /user=<user> /password=<password> [/autorun=false]", myName, SettingsKeyName);
  writer->Write      (L"      installs service to the system to run under given user/password ");
  writer->Write();
  writer->WriteFormat(L"    %s /%s=<path to settings file> /runAsSystem [/autorun=false]", myName, SettingsKeyName);
  writer->Write      (L"      installs service to the system to run under local system");
}

int CreateServiceAction::ExecuteAction(const Argz* az, const RunServiceSettings* baseSettings) {  
  CString serviceCommand;
  int ret = ServiceAction().GenerateServiceCommandLine(az, serviceCommand);
  if (ret != 0) return ret;
  
  SimpleCreateServiceSettings settings(baseSettings, serviceCommand);

  if (az->HasArgument(L"/runAsSystem")) {
    settings.setRunAsSystem(true);
  } else {
    CString p;
    if (!az->GetNamedArgument(L"user", p)) {
      LOG.LogWarn(L"/user=<user> parameter must be specified.");
      return 1;
    }
    settings.setUserName(p);
    if (!az->GetNamedArgument(L"password", p)) {
      LOG.LogWarn(L"/password=<password> parameter must be specified.");
      return 1;
    }
    settings.setPassword(p);
  }

  CString p(L"true");
  az->GetNamedArgument(L"autorun", p);
  if (p.CompareNoCase(L"false") == 0) {
    settings.setAutostart(false);
  } 
  if (p.CompareNoCase(L"true") == 0) {
    settings.setAutostart(true);
  }

  CreateServiceCommand cmd(&settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}
