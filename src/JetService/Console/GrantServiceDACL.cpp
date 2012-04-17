#include "StdAfx.h"
#include "GrantServiceDACL.h"
#include "Logger.h"
#include "ServiceGrantDACLCommand.h"

const Logger LOG(L"GrantServiceDACL");


GrantServiceDACL::GrantServiceDACL() : CreateServiceSettingsAction(L"grantService")
{
}


GrantServiceDACL::~GrantServiceDACL()
{
}

void GrantServiceDACL::PrintUsage(ConsoleWriter* writer) {  
  writer->WriteFormat(L"    %s /%s=<path to settings file> %s", myName, SettingsKeyName, GetUserArgumentsList());
  writer->Write      (L"      gives user rights to start/stop the service");
  writer->Write();
}


int GrantServiceDACL::ExecuteAction(const Argz* az, const CreateServiceSettings* settings, const ServiceTaskSettings* task) {  
  if (settings->runAsSystem()) return 0;

  ServiceGrantDACLCommand cmd(settings);
  int ret = static_cast<Command*>(&cmd)->executeCommand();
  if (ret != 0) {
    LOG.LogWarn(L"Failed to grant rights for user.");    
  }  
  return 0;
}

