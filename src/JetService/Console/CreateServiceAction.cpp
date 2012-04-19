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

const CString CreateServiceAction::KEY_DACL = L"changeDACL";

void CreateServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s /%s=<path to settings file> %s [/autostart=false] %s [%s]", myName, SettingsKeyName, GetUserArgumentsList(), GetUsageString(), Argz::MakeBoolArgument(KEY_DACL, true));
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
    if (az->GetBooleanArgument(KEY_DACL, true)) {
      ServiceGrantDACLCommand cmd(settings);
      int ret = static_cast<Command*>(&cmd)->executeCommand();
      if (ret != 0) {
        LOG.LogWarn(L"Failed to grant rights for user.");
        LOG.LogWarnFormat(L"To disable this check add '%s' commandline parameter", Argz::MakeBoolArgument(KEY_DACL));
      }
    }
  }

  LOG.LogInfo      (L"");
  LOG.LogInfo      (L"Use the folloging commands to start/stop the service:");
  LOG.LogInfoFormat(L"  net.exe start %s", settings->getServiceName());
  LOG.LogInfo      (L"    to start service");
  LOG.LogInfo      (L"");
  LOG.LogInfoFormat(L"  net.exe stop %s", settings->getServiceName());
  LOG.LogInfo      (L"    to stop service");
  LOG.LogInfo      (L"");

  return 0;
}
