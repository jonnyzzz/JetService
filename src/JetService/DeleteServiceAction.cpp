#include "StdAfx.h"
#include "DeleteServiceAction.h"
#include "DeleteServiceCommand.h"
#include "FileServiceSettings.h"
#include "Logger.h"

const Logger LOG(L"DeleteServiceAction");


DeleteServiceAction::DeleteServiceAction()
  : ConsoleAction(L"delete") 
{
}


DeleteServiceAction::~DeleteServiceAction(void)
{
}


CString DeleteServiceAction::GetUsage() {
  return L"";
}


class DeleteFileSettings : public FileServiceSettings {
public:
  DeleteFileSettings(CString name) : FileServiceSettings(name) {}
  virtual ~DeleteFileSettings() {}

public:
  virtual int executeCommand(const ServiceSettings* settings);

};

int DeleteFileSettings::executeCommand(const ServiceSettings* settings) {
  DeleteServiceCommand cmd(settings);
  Command* pcmd = &cmd;
  return pcmd->executeCommand();
}


int DeleteServiceAction::ExecuteAction(const Argz* az){
  CString file;
  if (az->GetNamedArgument(L"settings", file)) {
    LOG.LogErrorFormat(L"Failed to find configuration file");
    return 1;
  }

  DeleteFileSettings cmd(file);
  return ((Command*)&cmd)->executeCommand();
}
