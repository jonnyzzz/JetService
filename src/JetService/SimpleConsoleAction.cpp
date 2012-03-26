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
  CString path = L"";
  if (!az->GetNamedArgument(SettingsKeyName, path)) {  
    LOG.LogErrorFormat(L"Failed to find configuration file");
    return 1;
  }

  const DWORD sz = 65535;
  TCHAR buff[sz+1];
  DWORD n = GetFullPathName(path, sz, buff, NULL);
  if (n <= 0 || n >= sz) {
    LOG.LogErrorFormat(L"Failed to compute full path for settings file: %s", path);    
    return 1;
  }

  result.Append(buff);
  return 0;
}

int SimpleConsoleAction::ExecuteAction(const Argz* az) {
  CString file;
  int ret = GetSettingsFile(az, file);
  if (ret != 0) return ret;

  FileSettingsWrapper cmd(file, this, az);
  return static_cast<Command*>(&cmd)->executeCommand();
}