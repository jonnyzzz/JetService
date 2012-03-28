#include "StdAfx.h"
#include "DeleteServiceAction.h"
#include "DeleteServiceCommand.h"
#include "FileTaskSettings.h"
#include "Logger.h"

const Logger LOG(L"DeleteServiceAction");


DeleteServiceAction::DeleteServiceAction()
  : SimpleConsoleAction(L"delete") 
{
}


DeleteServiceAction::~DeleteServiceAction(void)
{
}

class ValidateServiceTaskSettings : public FileTaskSettings {
public:

  ValidateServiceTaskSettings(const RunServiceSettings* settings) 
    : FileTaskSettings(settings) {}
  virtual ~ValidateServiceTaskSettings() {}

public:
  virtual int executeCommand(const ServiceTaskSettings* settings) {
    LOG.LogInfoFormat(L"Program to execute: %s", settings->getProgramPath());
    LOG.LogInfoFormat(L"Program arguments:  %s", settings->GetProgramArguments());
    LOG.LogInfoFormat(L"Program work dir:   %s", settings->getWorkDir());
    return 0;
  }
};


void DeleteServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s /%s=<path to settings file>", myName, SettingsKeyName);
  writer->Write(      L"      removes installed service");  
}

int DeleteServiceAction::ExecuteAction(const Argz* az, const RunServiceSettings* settings){

  ValidateServiceTaskSettings validate(settings);

  int ret = static_cast<Command*>(&validate)->executeCommand();
  if (ret != 0) return ret;

  DeleteServiceCommand cmd(settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}
