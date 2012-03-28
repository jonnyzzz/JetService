#pragma once

#include "ConsoleAction.h"
#include "ServiceSettings.h"

class SimpleConsoleAction : public ConsoleAction
{
public:
  SimpleConsoleAction(const CString& name);
  virtual ~SimpleConsoleAction(void);

public:    
  virtual int ExecuteAction(const Argz* argz);

protected:
  int GetSettingsFile(const Argz* argz, CString& result);

public:
  virtual void PrintUsage(ConsoleWriter* writer) = 0;  
  virtual int ExecuteAction(const Argz* argz, const RunServiceSettings* settings) = 0;

public:
  static const CString SettingsKeyName;

};

