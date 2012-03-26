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
  writer->WriteFormat(L"    %s /%s=<path to settings file>", myName, SettingsKeyName);
  writer->Write(      L"      removes installed service");  
}

int DeleteServiceAction::ExecuteAction(const Argz* az, const ServiceSettings* settings){
  DeleteServiceCommand cmd(settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}
