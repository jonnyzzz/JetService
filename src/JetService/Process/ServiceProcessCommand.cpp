#include "StdAfx.h"
#include "ServiceProcessCommand.h"
#include "ProcessInterruptTerminateHandler.h"
#include "ProcessInterruptConsoleControlHandler.h"
#include "ProcessInterruptCallProcessHandler.h"
#include "Logger.h"

const Logger LOG(L"ServiceProcessCommand");


ServiceProcessCommand::ServiceProcessCommand(const ServiceTaskSettings* settings, InterruptHolder* interrupt)
  : InterruptHolder(interrupt)
  , mySettings(settings)
{
}


ServiceProcessCommand::~ServiceProcessCommand(void)
{
}

void ServiceProcessCommand::resetEnvironmentVariables2() {
  LOG.LogDebug(L"Start reading environment");

  CString path = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
  CRegKey reg(HKEY_LOCAL_MACHINE);
  if (ERROR_SUCCESS != reg.Open(reg, path, KEY_READ)) {
    LOG.LogWarnFormat(L"Failed to open environment variables section in registry. %s", LOG.GetLastError());
    return;
  }
  
  const DWORD sz = 65535;
  TCHAR buff[sz+1];

  const std::vector<CString>* keys = mySettings->getEnvironmentVariablesToOverride();  
  for(std::vector<CString>::const_iterator it = keys->begin(); it != keys->end(); ++it) {
    CString key(*it);
    
    ULONG chars = sz;
    if (ERROR_SUCCESS != reg.QueryStringValue(key, buff, &chars)) {
      LOG.LogDebugFormat(L"Failed to find %s system environment variable. %s", key, LOG.GetLastError());
      continue;
    }
    CString value;
    buff[chars] = L'\0';
    value.Append(buff);
    if (TRUE != SetEnvironmentVariable(key, value)) {
      LOG.LogDebugFormat(L"Failed to set environment variable %s=%s for process. %s", key, value, LOG.GetLastError());
    } else {
      LOG.LogDebugFormat(L"Set environment variable %s=%s for process", key, value);
    }
  }
}


int ServiceProcessCommand::executeCommand() {  
  LOG.LogDebug(L"starting process");  
  resetEnvironmentVariables2();

  long l =  mySettings->getTerminateWaitTimeoutMilliseconds();
  LOG.LogDebugFormat(L"stop wait timeout: %ld", l);

  ProcessInterruptTerminateHandler inthndl;  
  ProcessInterruptCallProcessHandler stopAction(mySettings);

  ProcessInterruptHandler* handler;
  if (mySettings->getStopCommand() != NULL) {    
    LOG.LogDebugFormat(L"Will call stop command. Termination wait timeout is %ld milliseconds.", mySettings->getTerminateWaitTimeoutMilliseconds());
    handler = &stopAction;
  } else {
    LOG.LogDebug(L"Process will be terminated on service exit");
    handler = &inthndl;
  }

  ProcessCommand cmd(mySettings, handler, this);
  return static_cast<Command*>(&cmd)->executeCommand();
}

