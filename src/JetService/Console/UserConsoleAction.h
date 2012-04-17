#pragma once

#include "ServiceSettings.h"
#include "ConsoleAction.h"

class SimpleUserSettings;


class UserSettingsParser {
public:
  int ParseUserSettings(const Argz* argz, SimpleUserSettings* settings);
  CString GetUserArgumentsList();
};

class UserConsoleAction : public ConsoleAction, protected UserSettingsParser
{
public:
  UserConsoleAction(const CString& commandName);
  virtual ~UserConsoleAction();

public:    
  virtual int ExecuteAction(const Argz* argz);
  virtual int ExecuteAction(const Argz* argz, UserSettings* settings) = 0;
};


