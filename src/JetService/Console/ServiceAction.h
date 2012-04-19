#pragma once
#include "SimpleConsoleAction.h"
#include "InstanceCount.h"
#include "JetServiceMain.h"

class ServiceAction : public SimpleConsoleAction, private InstanceCount<ServiceAction>
{
public:
  ServiceAction();
  virtual ~ServiceAction();

public:
  virtual void PrintUsage(ConsoleWriter* writer);  
  virtual int ExecuteAction(const Argz* argz, const ServiceSettings* settings);

public:
  int GenerateServiceCommandLine(const Argz* argz, CString& result, CString& logFile);

public:
  static const CString JetServiceCommand;
  void JetServiceMain(const Argz* argz, const ServiceSettings* settings, DWORD dwArgc, LPTSTR *lpszArgv);

private:
  JetServiceState<3> myState;
};

