#include "StdAfx.h"
#include "DeleteByNameAction.h"
#include "Logger.h"
#include "SimpleServiceSettings.h"
#include "DeleteServiceCommand.h"

const Logger LOG(L"DeleteByNameAction");

DeleteByNameAction::DeleteByNameAction() : ConsoleAction(L"deleteByName")
{
}


DeleteByNameAction::~DeleteByNameAction()
{
}

const CString DeleteByNameAction::KEY_SERVICE_NAME = L"service";

void DeleteByNameAction::PrintUsage(ConsoleWriter* writer) {
  if (LOG.IsDebugEnabled()) {
    writer->WriteFormat(L"    %s /%s=<path to settings file>", myName, KEY_SERVICE_NAME);
    writer->Write(      L"      removes installed service by given name");  
    writer->Write();
  }
}

int DeleteByNameAction::ExecuteAction(const Argz* az){
  CString name;
  if (!az->GetNamedArgument(KEY_SERVICE_NAME, name)) {
    LOG.LogErrorFormat(L"Failed to find argument %s", KEY_SERVICE_NAME);
    return 1;
  }

  SimpleServiceSettings settings(name, name, name, L"");  
  DeleteServiceCommand cmd(&settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}
