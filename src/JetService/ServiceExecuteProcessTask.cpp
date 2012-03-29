#include "StdAfx.h"
#include "ServiceExecuteProcessTask.h"
#include "ServiceEventContext.h"
#include "ServiceStatus.h"
#include "Logger.h"

const Logger LOG(L"ServiceExecuteProcessTask");

ServiceExecuteProcessTask::ServiceExecuteProcessTask(const ServiceEventContext* context)
  : SettingsServiceTask(context)
{
}


ServiceExecuteProcessTask::~ServiceExecuteProcessTask()
{
}

void ServiceExecuteProcessTask::ExecuteProcess() {
  //make explicit
  SettingsServiceTask::ExecuteProcess();
}

HANDLE ServiceExecuteProcessTask::CreateProcessToken() {
  HANDLE threadHandle = GetCurrentThread();

  HANDLE ourToken;  
  if (0 == OpenThreadToken(threadHandle, TOKEN_DUPLICATE , TRUE, &ourToken)) {
    LOG.LogErrorFormat(L"Failed to OpenThreadToken. %s", LOG.GetLastError());    
    return NULL;
  }

  HANDLE startProcessToken;
  if (0 == DuplicateTokenEx(
    ourToken, 
    MAXIMUM_ALLOWED,
    NULL,
		SecurityIdentification, 
    TokenPrimary,
    &startProcessToken)) {
      LOG.LogErrorFormat(L"Failed to DuplicateTokenEx. %s", LOG.GetLastError());      
      return NULL;
  }

  CloseHandle(ourToken);
  CloseHandle(threadHandle);
  return startProcessToken;
}


STARTUPINFO ServiceExecuteProcessTask::CreateProcessStartupInfo() {
  STARTUPINFO info;
  ZeroMemory(&info, sizeof(info));
  info.cb = sizeof(info);
  //TODO: Create pipes and transfer process output.
  //info.dwFlags = STARTF_USESTDHANDLES;

  return info;
}

void ServiceExecuteProcessTask::ExecuteProcess(const ServiceTaskSettings* settings) {
  HANDLE processToken = CreateProcessToken();
  if (processToken == NULL) {
    myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
    return;
  }

  CString commandLine;
  CString workdir = settings->getWorkDir();
  commandLine.AppendFormat(L"\"%s\" %s", settings->getProgramPath(), settings->GetProgramArguments());
  LOG.LogDebugFormat(L"Starting process: %s (in %s)", commandLine, workdir);

  const int sz = 65535;
  TCHAR commandLineBuff[sz+1];
  CopyStringChars(commandLine, commandLineBuff, sz);

  //NOTE: it may be necessary to call LoadUserProfile beforehand

  STARTUPINFO startupInfo = CreateProcessStartupInfo();
  PROCESS_INFORMATION processInfo;

  LOG.LogDebug(L"Starting process");
  if (0 == CreateProcessAsUser(
    processToken, 
    NULL,
    commandLineBuff, //buffer may be changed!
    NULL, //process attributes
    NULL, //thread attributes
    TRUE, //inherit handles
    DETACHED_PROCESS | CREATE_UNICODE_ENVIRONMENT |  CREATE_NO_WINDOW, //TODO: | CREATE_NEW_PROCESS_GROUP
    NULL,
    workdir,
    &startupInfo,
    &processInfo)) {
      LOG.LogErrorFormat(L"Failed to CreateProcessAsUser: %s", LOG.GetLastError());
      myContext->GetServiceStatus()->SetStatus(StatusValue::STOPPED);
      return;
  }

  while(true) {    
    if (IsInterrupted()) {
      LOG.LogInfo(L"Terminating process. Interrupted flag is set");
      //TODO:implement kill process
      TerminateProcess(processInfo.hProcess, 42);
      //this is asynchronious process. we need to wait for a while for process to exit.
    }

    DWORD waitResult = WaitForSingleObject(processInfo.hProcess, 500);
    if (waitResult == WAIT_OBJECT_0) {
      //process is exited
      break;
    }
  }

  DWORD code;
  if (0 != GetExitCodeProcess(processInfo.hProcess, &code)) {    
    LOG.LogInfoFormat(L"Process exited with code: %d", code);
  } else {
    LOG.LogInfoFormat(L"Failed to get process exited code");
  }

  CloseHandle(processInfo.hProcess);
  CloseHandle(processInfo.hThread);
}

