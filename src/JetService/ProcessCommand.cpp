#include "StdAfx.h"
#include "ProcessCommand.h"
#include "Pipe.h"
#include "Logger.h"

const Logger LOG(L"ProcessCommand");

ProcessCommand::ProcessCommand(const ServiceTaskSettings* settings, const InterruptHolder* interrupt)
  : mySettings(settings)
  , myInterrupt(interrupt)
{
}

ProcessCommand::~ProcessCommand(void)
{
}

bool ProcessCommand::IsInterrupted() {  
  return myInterrupt->IsInterrupted();
}

HANDLE ProcessCommand::CreateProcessToken() {
  HANDLE threadHandle = GetCurrentProcess();

  HANDLE ourToken;  
  if (0 == OpenProcessToken(threadHandle, TOKEN_DUPLICATE , &ourToken)) {
    LOG.LogErrorFormat(L"Failed to OpenProcessToken. %s", LOG.GetLastError());    
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


STARTUPINFO ProcessCommand::CreateProcessStartupInfo(ChildProcessHandle* pstdin, ChildProcessHandle* pstdout, ChildProcessHandle* pstderr) {
  STARTUPINFO info;
  ZeroMemory(&info, sizeof(info));
  info.cb = sizeof(info);
  //TODO: Create pipes and transfer process output.
  info.dwFlags = STARTF_USESTDHANDLES;
  info.hStdOutput = pstdout->GetChildProcessHandle();
  info.hStdError = pstderr->GetChildProcessHandle();
  info.hStdInput = pstdin->GetChildProcessHandle();
  return info;
}

int ProcessCommand::executeCommand() {
  LOG.LogDebug(L"Starting process");

  HANDLE processToken = CreateProcessToken();
  if (processToken == NULL) {    
    return 1;
  }

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

  STARTUPINFO startupInfo = CreateProcessStartupInfo(&stdInHandle, &stdOutHandle, &stdOutHandle);
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
      return 1; 
  }

  //avoid handle duplication in current process
  stdInHandle.CloseChildProcessHandle();  
  stdOutHandle.CloseChildProcessHandle();

  //closee handles as we do not like to process process output
  stdInHandle.CloseHostProcessHandle();
  stdOutHandle.CloseHostProcessHandle();

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
  return 0;
}

