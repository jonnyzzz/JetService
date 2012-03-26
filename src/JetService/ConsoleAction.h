#pragma once
#include "Argz.h"
#include "ConsoleWriter.h"

class ConsoleAction
{
public:
  ConsoleAction(const CString& name);
  virtual ~ConsoleAction();

public:
  CString GetCommandName() const;
  virtual void PrintUsage(ConsoleWriter* writer) = 0;  
  virtual int ExecuteAction(const Argz* argz) = 0;

protected:
  const CString myName;
};

