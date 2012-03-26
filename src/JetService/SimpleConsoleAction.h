#pragma once

#include "ConsoleAction.h"
#include "ServiceSettings.h"

class SimpleConsoleAction : public ConsoleAction
{
public:
  SimpleConsoleAction(const CString& name);
  virtual ~SimpleConsoleAction(void);

public:  
  virtual void PrintUsage(ConsoleWriter* writer) = 0;  
  virtual int ExecuteAction(const Argz* argz);

  virtual int ExecuteAction(const Argz* argz, const ServiceSettings* settings) = 0;

public:
  static const CString SettingsKeyName;

};

