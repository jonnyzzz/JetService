#pragma once
#include "Command.h"

class SCM_Right {
private:
  const DWORD myRight;
  const CString myName;
  SCM_Right(CString name, DWORD right) : myRight(right), myName(name) {}
public:
  operator DWORD() const { return myRight; }
  operator CString() const { return myName;}
  
  static const SCM_Right ALL_ACCESS; //(SC_MANAGER_ALL_ACCESS)
  static const SCM_Right CREATE_SERVICE; //SC_MANAGER_CREATE_SERVICE
  static const SCM_Right ENUMERATE_SERVICE; //SC_MANAGER_ENUMERATE_SERVICE
};

class ServiceControlManagerCommand : public Command
{
public:
  ServiceControlManagerCommand(const SCM_Right& right) : myRight(right) {}
  virtual ~ServiceControlManagerCommand() {}

public:
  virtual int executeCommand();
  virtual int executeCommand(SC_HANDLE scm) = 0;

private: 
  const SCM_Right& myRight;  
};

