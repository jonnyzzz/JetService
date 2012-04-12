#include "StdAfx.h"
#include "ProcessAction.h"
#include "ProcessCommand.h"
#include "FileTaskSettings.h"


ProcessAction::ProcessAction(void)
  : SimpleConsoleAction(L"console") 
{
}


ProcessAction::~ProcessAction(void)
{
}

void ProcessAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s /%s=<path to settings file>", myName, SettingsKeyName);
  writer->Write(      L"      starts service process in console window");  
}

class Settings : public FileTaskSettings {
public:
  Settings(const ServiceSettings* settings) : FileTaskSettings(settings) {}
  virtual ~Settings() {}

public:
  int executeCommand(const ServiceTaskSettings* settings) {
    //TODO: Register console control handler to stop on CTRL+C or CTRL+BREAK commands
    InterruptHolder interrupt;
    ProcessCommand cmd(settings, &interrupt);
    return static_cast<Command*>(&cmd)->executeCommand();
  }
};

int ProcessAction::ExecuteAction(const Argz* az, const ServiceSettings* settings){
  Settings cmd(settings);
  return static_cast<Command*>(&cmd)->executeCommand();
}

