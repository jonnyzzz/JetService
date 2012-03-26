#pragma once
#include "Argz.h"

class ConsoleAction
{
public:
  ConsoleAction(const CString& name);
  virtual ~ConsoleAction();

public:
  CString GetCommandName() const;
  virtual CString GetUsage() = 0;  
  virtual int ExecuteAction(const Argz* argz) = 0;

protected:
  const CString myName;
};

