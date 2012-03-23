#pragma once

#include "ServiceControlManagerCommand.h"

class ServiceSettings {
public:
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual bool isAutostart() const;

  virtual bool runAsSystem() const;
  virtual CString getUserName() const;
  virtual CString getPassword() const;
};

class CreateServiceCommand : public ServiceControlManagerCommand
{
public:
  CreateServiceCommand(const ServiceSettings* settings);
  virtual ~CreateServiceCommand();

public:
  virtual int executeCommand(SC_HANDLE scm);


private:
  const ServiceSettings* mySettings;

};

