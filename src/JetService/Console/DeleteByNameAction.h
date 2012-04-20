#pragma once

#include "ConsoleAction.h"

class DeleteByNameAction : public ConsoleAction
{
public:
  DeleteByNameAction();
  virtual ~DeleteByNameAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz);

private:
  static const CString KEY_SERVICE_NAME;
};


