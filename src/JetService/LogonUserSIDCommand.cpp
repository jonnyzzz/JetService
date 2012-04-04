#include "StdAfx.h"
#include "LogonUserSIDCommand.h"
#include "Logger.h"

const Logger LOG(L"LogonUserSIDCommand");

LogonUserSIDCommand::LogonUserSIDCommand(const CreateServiceSettings* settings)
  : LogonUserCommand(settings, LogonUserMode::NETWORK)
  , mySettings(settings)
{
}


LogonUserSIDCommand::~LogonUserSIDCommand()
{
}


int LogonUserSIDCommand::executeCommand(HANDLE userToken) {
  DWORD bufferSize = 0;
  BOOL b = GetTokenInformation(
         userToken,         // handle to the access token
         TokenGroups,    // get information about the token's groups 
         NULL,   // pointer to TOKEN_GROUPS buffer
         0,              // size of buffer
         &bufferSize       // receives required buffer size
      );

  LOG.LogDebugFormat(L"Required size for GetTokenInformation is: %d bytes", bufferSize);
  if (b == TRUE || GetLastError() != ERROR_INSUFFICIENT_BUFFER)  {
    LOG.LogWarnFormat(L"Failed to get required buffer size for GetTokenInformation. %s", LOG.GetLastError());
    return 1;
  }
  bufferSize *= 4;  
  PTOKEN_GROUPS buffer = (PTOKEN_GROUPS)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize + 1);  
  int ret = executeCommand(userToken, buffer, bufferSize);
  HeapFree(GetProcessHeap(), 0, buffer);
  return ret;
}


int LogonUserSIDCommand::executeCommand(HANDLE userToken, PTOKEN_GROUPS groups, DWORD size) {
  DWORD actual = size;
   // Get the token group information from the access token.
   if (0 == GetTokenInformation(
         userToken,         // handle to the access token
         TokenGroups,    // get information about the token's groups 
         (LPVOID) groups,   // pointer to TOKEN_GROUPS buffer
         size,       // size of buffer
         &actual       // receives required buffer size
         )) {
     LOG.LogWarnFormat(L"Failed to GetTokenInformation. %s", LOG.GetLastError());
     return 1;
   }

   LOG.LogDebugFormat(L"Fetched %d TokenGroups", groups->GroupCount);

  // Loop through the groups to find the logon SID.  
  for (DWORD i = 0; i < groups->GroupCount; i++) {
    SID_AND_ATTRIBUTES& attr = groups->Groups[i];
    if ((attr.Attributes & SE_GROUP_LOGON_ID) != SE_GROUP_LOGON_ID) continue;

    return executeCommandCopySID(userToken, attr.Sid);
  }

  LOG.LogWarn(L"Failed to find user SID");
  return 1;
}


int LogonUserSIDCommand::executeCommandCopySID(HANDLE userToken, PSID sid) {
  DWORD sz = 4 * GetLengthSid(sid);
  PSID copy = (PSID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sz);
  
  int ret;
  if (0 == CopySid(sz, copy, sid)) {
    LOG.LogError(L"Failed to copy SID");
    ret = 1;
  } else {    
    ret = executeCommand(userToken, copy);
  }

  HeapFree(GetProcessHeap(), 0, copy);
  return ret;
}

