#pragma once

#include "ServiceCommand.h"
#include "ServiceSettings.h"

class ServiceGrantDACLCommand : public ServiceCommand
{
public:
  ServiceGrantDACLCommand(const CreateServiceSettings* settings);
  virtual ~ServiceGrantDACLCommand(void);

public:
  virtual int executeCommand(SC_HANDLE serviceControlManager, SC_HANDLE service);

private:
  int executeCommand(SC_HANDLE serviceControlManager, SC_HANDLE service, PSECURITY_DESCRIPTOR descr, DWORD descrSZ);
  int executeCommand(
    SC_HANDLE serviceControlManager, 
    SC_HANDLE service, 
    PSECURITY_DESCRIPTOR descr, 
    EXPLICIT_ACCESS& ea,
    PACL oldAcl,
    PACL newAcl    
    );

private:
  const CreateServiceSettings* const mySettings;
};

