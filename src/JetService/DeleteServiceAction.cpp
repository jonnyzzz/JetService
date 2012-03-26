#include "StdAfx.h"
#include "DeleteServiceAction.h"
#include "DeleteServiceCommand.h"
#include "FileServiceSettings.h"
#include "Logger.h"

const Logger LOG(L"DeleteServiceAction");


DeleteServiceAction::DeleteServiceAction()
  : SimpleConsoleAction(L"delete") 
{
}


DeleteServiceAction::~DeleteServiceAction(void)
{
}


void DeleteServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    delete /%s=<path to settings file>", SettingsKeyName);
  writer->Write(L"      removes installed service");  
}

int DeleteServiceAction::ExecuteAction(const Argz* az, const ServiceSettings* settings){
  CString file;
  if (az->GetNamedArgument(L"settings", file)) {
    LOG.LogErrorFormat(L"Failed to find configuration file");
    return 1;
  }

  DeleteServiceCommand cmd(settings);
  return ((Command*)&cmd)->executeCommand();
}
