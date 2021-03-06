#pragma once

#include "ServiceSettings.h"
#include "ServiceControlManagerCommand.h"

class SC_Right {
private:
  const SCM_Right& mySCMRight;
  const DWORD myRight;
  const CString myName;
  SC_Right(CString name, const SCM_Right& scm, DWORD right) 
    : myRight(right)
    , mySCMRight(scm)
    , myName(name) {}

public:
  operator DWORD() const { return myRight; }
  operator CString() const { return myName;}
  operator const SCM_Right&() const { return mySCMRight;}
  
  static const SC_Right ALL_ACCESS; //SERVICE_ALL_ACCESS
  static const SC_Right SERVICE_DELETE; //DELETE
  static const SC_Right MODIFY_DACL; 
  //static const SC_Right CHANGE_CONFIG; //SERVICE_CHANGE_CONFIG 
  //static const SC_Right INTERROGATE; //SERVICE_INTERROGATE
  //static const SC_Right QUERY_CONFIG; //SERVICE_QUERY_CONFIG
  //static const SC_Right START; //SERVICE_SERVICE_START 
  //static const SC_Right STOP; //SERVICE_SERVICE_STOP
  //static const SC_Right USER_DEFINED; //SERVICE_USER_DEFINED_CONTROL
};


class ServiceCommand : public ServiceControlManagerCommand
{
public:
  ServiceCommand(const ServiceSettings* settings, const SC_Right& sRight);
  virtual ~ServiceCommand(void);

public:
  virtual int executeCommand(SC_HANDLE serviceControlManager);
  virtual int executeCommand(SC_HANDLE serviceControlManager, SC_HANDLE service) = 0;

protected:
  const ServiceSettings* mySettings;

private:
  const SC_Right& mySCRight;
};

