#include "StdAfx.h"
#include "ServiceGrantDACLCommand.h"
#include "Logger.h"

const Logger LOG(L"ServiceGrantDACLCommand");


ServiceGrantDACLCommand::ServiceGrantDACLCommand(const CreateServiceSettings* settings)
  : ServiceCommand(settings, SC_Right::MODIFY_DACL)
  , mySettings(settings)
{
}


ServiceGrantDACLCommand::~ServiceGrantDACLCommand(void)
{
}


int ServiceGrantDACLCommand::executeCommand(SC_HANDLE serviceControlManager, SC_HANDLE service) {
  if (mySettings->runAsSystem()) return 0;

  LOG.LogDebug(L"Going to give service running user account rights to start/stop service");

  DWORD buffSize = 0;
  if (0 != QueryServiceObjectSecurity(service, DACL_SECURITY_INFORMATION, NULL, 0, &buffSize) 
    || GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
    LOG.LogErrorFormat(L"Failed to get service object security. %s", LOG.GetLastError());
    return 1;
  }

  PSECURITY_DESCRIPTOR descr = (PSECURITY_DESCRIPTOR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffSize + 1);
  int ret = executeCommand(serviceControlManager, service, descr, buffSize);
  HeapFree(GetProcessHeap(), 0, (LPVOID)descr);
  return ret;
}


int ServiceGrantDACLCommand::executeCommand(SC_HANDLE serviceControlManager, SC_HANDLE service, PSECURITY_DESCRIPTOR descr, DWORD descrSZ) {
  DWORD buffSize = 0;
  if (0 == QueryServiceObjectSecurity(service, DACL_SECURITY_INFORMATION, descr, descrSZ, &buffSize)) {
    LOG.LogErrorFormat(L"Failed to get service object security. %s", LOG.GetLastError());
    return 1;
  }

  LOG.LogDebug(L"Got ServiceObjectSecurity");

  BOOL daclPresent;
  BOOL daclDefault;
  PACL pacl;
  if (0 == GetSecurityDescriptorDacl(descr, &daclPresent, &pacl, &daclDefault)) {
    LOG.LogErrorFormat(L"Failed to get security descriptor DACL. %s", LOG.GetLastError());
    return 1;
  }

  EXPLICIT_ACCESS ea;
  const DWORD sz = 32768;
  TCHAR userName[sz+1];
  if (!CopyStringChars(mySettings->getUserName(), userName, sz)) {
    LOG.LogError(L"User name is too long");
    return 1;
  }

  BuildExplicitAccessWithName(&ea, userName,
        SERVICE_START | SERVICE_STOP | READ_CONTROL | DELETE,
        SET_ACCESS, NO_INHERITANCE);

  PACL newAcl;
  if (ERROR_SUCCESS != SetEntriesInAcl(1, &ea, pacl, &newAcl)) {
    LOG.LogErrorFormat(L"Failed to set entries in ACL. %s", LOG.GetLastError());
    return 1;
  }

  int ret = executeCommand(serviceControlManager, service, descr, ea, pacl, newAcl);
  LocalFree((HLOCAL)newAcl);
  return ret;
}

int ServiceGrantDACLCommand::executeCommand(
    SC_HANDLE serviceControlManager, 
    SC_HANDLE service, 
    PSECURITY_DESCRIPTOR descr, 
    EXPLICIT_ACCESS& ea,
    PACL oldAcl,
    PACL newAcl) {

  SECURITY_DESCRIPTOR sd;
  if (0 == InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) {
    LOG.LogErrorFormat(L"Failed to initialize Security Descriptior. %s", LOG.GetLastError());
    return 1;
  }

  if (0 == SetSecurityDescriptorDacl(&sd, TRUE, newAcl, FALSE)) {
    LOG.LogErrorFormat(L"Failed to set Security Descriptior DACL. %s", LOG.GetLastError());
    return 1;
  }

  if (0 == SetServiceObjectSecurity(service, DACL_SECURITY_INFORMATION, &sd)) {
    LOG.LogErrorFormat(L"Failed to set Security DACL for service. %s", LOG.GetLastError());
    return 1;
  }

  LOG.LogInfo(L"Allowed service running user to start, stop the service");
  return 0;
}
