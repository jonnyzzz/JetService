#include "StdAfx.h"
#include "CreateServiceAction.h"
#include "CreateServiceCommand.h"
#include "ServiceAction.h"
#include "FileTaskSettings.h"
#include "Logger.h"

const Logger LOG(L"CreateServiceAction");


CreateServiceAction::CreateServiceAction(void) : CreateServiceSettingsAction(L"install")
{
}


CreateServiceAction::~CreateServiceAction(void)
{
}


void CreateServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s /%s=<path to settings file> /user=<user> [/domain=<domain>] /password=<password> [/autostart=false] [/checkUserAccount=false]", myName, SettingsKeyName);
  writer->Write      (L"      installs service to the system to run under given user/password ");
  writer->Write();
  writer->WriteFormat(L"    %s /%s=<path to settings file> /runAsSystem [/autostart=false]", myName, SettingsKeyName);
  writer->Write      (L"      installs service to the system to run under local system");
}


int CreateServiceAction::ExecuteAction(const Argz* az, const CreateServiceSettings* settings, const ServiceTaskSettings* task) {  
  CreateServiceCommand cmd(settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}
