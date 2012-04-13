#pragma once

#include "SimpleConsoleAction.h"


class DeleteServiceAction : public SimpleConsoleAction
{
public:
  DeleteServiceAction();
  virtual ~DeleteServiceAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const ServiceSettings* settings);

};

