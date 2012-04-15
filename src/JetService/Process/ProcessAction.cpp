#include "StdAfx.h"
#include "ProcessAction.h"
#include "ServiceProcessCommand.h"
#include "FileTaskSettings.h"
#include "Logger.h"

const Logger LOG(L"ProcessAction");


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
  Settings(const ServiceSettings* settings, ProcessAction* action) : FileTaskSettings(settings), myAction(action) {}
  virtual ~Settings() {}

public:
  int executeCommand(const ServiceTaskSettings* settings) {
    return myAction->ExecuteActionWithSettings(settings);
  }
private:
  ProcessAction* myAction;
};

int ProcessAction::ExecuteAction(const Argz* az, const ServiceSettings* settings){
  Settings cmd(settings, this);
  return static_cast<Command*>(&cmd)->executeCommand();
}

int ProcessAction::ExecuteActionWithConsoleHandler(const ServiceTaskSettings* settings) {
  ServiceProcessCommand cmd(settings, this);
  return static_cast<Command*>(&cmd)->executeCommand();
}

int ProcessAction::ExecuteActionWithSettings(const ServiceTaskSettings* settings) {
  bool needUnregister = true;
  if (0 == SetConsoleCtrlHandler(GetConsoleControlHandlerFunction(), TRUE)) {
    LOG.LogInfoFormat(L"Failed to register console control handler. %s", LOG.GetLastError());
    needUnregister = false;
  }

  int ret = ExecuteActionWithConsoleHandler(settings);

  if (needUnregister) {
    if (0 == SetConsoleCtrlHandler(GetConsoleControlHandlerFunction(), FALSE)) {
      LOG.LogInfoFormat(L"Failed to unregister console control handler. %s", LOG.GetLastError());    
    }
  }

  return ret;
}

BOOL ProcessAction::ConsoleControlHandler(DWORD dwCtrlType) {
  LOG.LogInfoFormat(L"Console Control event recieved: %d", dwCtrlType);
  Interrupt();
  return TRUE;
}

