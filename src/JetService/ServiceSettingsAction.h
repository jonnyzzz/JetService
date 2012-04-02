#pragma once

#include "SimpleConsoleAction.h"

class ServiceSettingsAction : public SimpleConsoleAction
{
public:
  ServiceSettingsAction(const CString& commandName);
  virtual ~ServiceSettingsAction(void);

public:
  virtual int ExecuteAction(const Argz* argz, const RunServiceSettings* settings);
  virtual int ExecuteAction(const Argz* argz, const ServiceTaskSettings* task) = 0;

};

