#include "StdAfx.h"
#include "SimpleConsoleAction.h"
#include "FileServiceSettings.h"
#include "Logger.h"

const Logger LOG(L"SimpleConsoleAction");
const CString SimpleConsoleAction::SettingsKeyName = L"settings";

SimpleConsoleAction::SimpleConsoleAction(const CString& name)
  : ConsoleAction(name)
{
}

SimpleConsoleAction::~SimpleConsoleAction(void)
{
}

class FileSettingsWrapper : public FileServiceSettings {
public:
  FileSettingsWrapper(CString name, SimpleConsoleAction* action, const Argz* argz) 
    : FileServiceSettings(name)
    , myAction(action)
    , myArgz(argz) {
  }
  virtual ~FileSettingsWrapper() {}

public:
  virtual int executeCommand(const ServiceSettings* settings);

private:
  SimpleConsoleAction* const myAction;
  const Argz* const myArgz;
};

int FileSettingsWrapper::executeCommand(const ServiceSettings* settings) {
  return myAction->ExecuteAction(myArgz, settings);
}

int SimpleConsoleAction::GetSettingsFile(const Argz* az, CString& result) {
  result = L"";
  if (!az->GetNamedArgument(SettingsKeyName, result)) {  
    LOG.LogErrorFormat(L"Failed to find configuration file");
    return 1;
  }
  return 0;
}

int SimpleConsoleAction::ExecuteAction(const Argz* az) {
  CString file;
  int ret = GetSettingsFile(az, file);
  if (ret != 0) return ret;

  FileSettingsWrapper cmd(file, this, az);
  return static_cast<Command*>(&cmd)->executeCommand();
}