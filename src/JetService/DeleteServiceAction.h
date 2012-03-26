#pragma once

#include "ConsoleAction.h"


class DeleteServiceAction : public ConsoleAction
{
public:
  DeleteServiceAction();
  virtual ~DeleteServiceAction();

public:
  virtual CString GetUsage();  
  virtual int ExecuteAction(const Argz* argz);

};

