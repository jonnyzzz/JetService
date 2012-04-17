#pragma once

#include "UserConsoleAction.h"

class GrantLogonAsServiceAction : public UserConsoleAction
{
public:
  GrantLogonAsServiceAction();
  virtual ~GrantLogonAsServiceAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, UserSettings* settings);
};

