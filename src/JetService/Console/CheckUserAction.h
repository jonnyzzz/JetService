#pragma once

#include "UserConsoleAction.h"

class CheckUserAction : public UserConsoleAction
{
public:
  CheckUserAction();
  virtual ~CheckUserAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, UserSettings* settings);
};


