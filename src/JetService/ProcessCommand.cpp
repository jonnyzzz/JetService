#include "StdAfx.h"
#include "ProcessCommand.h"
#include "Pipe.h"
#include "Logger.h"
#include "LogPipeReader.h"

const Logger LOG(L"ProcessCommand");

ProcessCommand::ProcessCommand(const ServiceTaskSettings* settings, InterruptHolder* interrupt)
  : mySettings(settings)
  , InterruptHolder(interrupt)
{
}

ProcessCommand::~ProcessCommand(void)
{
}

HANDLE ProcessCommand::CreateProcessStartUserToken() {
  HANDLE threadHandle = GetCurrentProcess();

  HANDLE ourToken;  
  if (0 == OpenProcessToken(threadHandle, TOKEN_DUPLICATE , &ourToken)) {
    LOG.LogDebugFormat(L"Failed to OpenProcessToken. %s", LOG.GetLastError());    
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
      LOG.LogDebugFormat(L"Failed to DuplicateTokenEx. %s", LOG.GetLastError());      
      return NULL;
  }

  CloseHandle(ourToken);
  CloseHandle(threadHandle);
  return startProcessToken;
}


STARTUPINFO ProcessCommand::CreateProcessStartupInfo(ChildProcessInHandle* pstdin, ChildProcessOutHandle* pout) {
  STARTUPINFO info;
  ZeroMemory(&info, sizeof(info));
  info.cb = sizeof(info);
  //TODO: Create pipes and transfer process output.
  info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  info.hStdOutput = pout->GetChildStdOutHandle();
  info.hStdError = pout->GetChildStdErrHandle();
  info.hStdInput = pstdin->GetChildProcessHandle();
  return info;
}

int ProcessCommand::executeCommand() {
  LOG.LogDebug(L"Starting process");

  HANDLE processToken = CreateProcessStartUserToken();
  if (processToken == NULL) {    
    LOG.LogErrorFormat(L"Failed to create process start user token"); 
    return 1;
  }

  int ret = DoStartProcessAsUser(processToken);
  CloseHandle(processToken);
  return ret;
}

int ProcessCommand::DoStartProcessAsUser(HANDLE processUserToken) {
  CString commandLine;
  CString workdir = mySettings->getWorkDir();
  commandLine.AppendFormat(L"\"%s\" %s", mySettings->getProgramPath(), mySettings->GetProgramArguments());
  LOG.LogDebugFormat(L"Starting process: %s (in %s)", commandLine, workdir);

  const int sz = 65535;
  TCHAR commandLineBuff[sz+1];
  CopyStringChars(commandLine, commandLineBuff, sz);

  //NOTE: it may be necessary to call LoadUserProfile beforehand

  ChildProcessInHandle stdInHandle;
  ChildProcessOutHandle stdOutHandle;

  if (!stdInHandle.IsValid() || !stdOutHandle.IsValid()) {
    LOG.LogError(L"Failed to setup pipes for process");
    return 1;
  }

  STARTUPINFO startupInfo = CreateProcessStartupInfo(&stdInHandle, &stdOutHandle);
  PROCESS_INFORMATION processInfo;

  LOG.LogDebug(L"Starting process");
  if (0 == CreateProcessAsUser(
    processUserToken, 
    NULL,
    commandLineBuff, //buffer may be changed!
    NULL, //process attributes
    NULL, //thread attributes
    TRUE, //inherit handles
    DETACHED_PROCESS | CREATE_UNICODE_ENVIRONMENT |  CREATE_NO_WINDOW | CREATE_NEW_PROCESS_GROUP, 
    NULL,
    workdir,
    &startupInfo,
    &processInfo)) {
      LOG.LogErrorFormat(L"Failed to CreateProcessAsUser: %s", LOG.GetLastError());      
      return 1; 
  }

  //avoid handle duplication in current process
  stdInHandle.CloseChildProcessHandle();  
  stdOutHandle.CloseChildProcessHandle();

  //closee handles as we do not like to process process output
  stdInHandle.CloseHostProcessHandle();

  int ret = WaitForProcessAndCaptureOutput(&stdOutHandle, processInfo);

  CloseHandle(processInfo.hProcess);
  CloseHandle(processInfo.hThread);  
  return ret;
}

int ProcessCommand::WaitForProcessAndCaptureOutput(ChildProcessOutHandle* stdOutHandle, PROCESS_INFORMATION& processInfo) {
  LogPipeReader outputReader(L"stdout");
  PipeReader outputRedirect(&outputReader, stdOutHandle->GetHostProcessHandle(), this);

  int ret = WaitForProcessToExit(processInfo);

  outputRedirect.WaitForExit();
  stdOutHandle->CloseHostProcessHandle();
  return ret;
}

int ProcessCommand::WaitForProcessToExit(PROCESS_INFORMATION& processInfo) {
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
  
  ///we ignore original exit code for now
  return 0;
}

