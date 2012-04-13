#include "StdAfx.h"
#include "CreateServiceAction.h"
#include "CreateServiceCommand.h"
#include "ServiceAction.h"
#include "FileTaskSettings.h"
#include "ServiceGrantDACLCommand.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceAction");


CreateServiceAction::CreateServiceAction(void) : CreateServiceCheckAccountAction(L"install")
{
}


CreateServiceAction::~CreateServiceAction(void)
{
}


void CreateServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s /%s=<path to settings file> /user=<user> [/domain=<domain>] /password=<password> [/autostart=false] [/checkUserAccount=false] [/giveUserRights=false] [/changeDACL=false]", myName, SettingsKeyName);
  writer->Write      (L"      installs service to the system to run under given user/password ");
  writer->Write();
  writer->WriteFormat(L"    %s /%s=<path to settings file> /runAsSystem [/autostart=false]", myName, SettingsKeyName);
  writer->Write      (L"      installs service to the system to run under local system");
}


int CreateServiceAction::ExecuteAction(const Argz* az, const CreateServiceSettings* settings, const ServiceTaskSettings* task) {  
  //check accounts
  {
    int ret = CreateServiceCheckAccountAction::ExecuteAction(az, settings, task);
    if (ret != 0) return ret;
  }

  //register service in the system
  {
    CreateServiceCommand cmd(settings);
    int ret = static_cast<Command*>(&cmd)->executeCommand();
    if (ret != 0) return ret;
  }


  if (!settings->runAsSystem()) {
    //give rights for the user to start/stop service
    if (az->GetBooleanArgument(L"changeDACL", true)) {
      ServiceGrantDACLCommand cmd(settings);
      int ret = static_cast<Command*>(&cmd)->executeCommand();
      if (ret != 0) {
        LOG.LogError(L"Failed to grant rights for user. To disable this check add /changeDACL=false commandline parameter");
      }
    }
  }

  return 0;
}
