#pragma once

#include "ConsoleAction.h"


class DeleteServiceAction : public ConsoleAction
{
public:
  DeleteServiceAction();
  virtual ~DeleteServiceAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz);

};

