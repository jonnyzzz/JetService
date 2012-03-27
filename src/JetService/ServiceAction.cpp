#include "StdAfx.h"
#include "ServiceAction.h"
#include "Logger.h"
#include "SimpleServiceSettings.h"
#include "ServiceMain.h"

const Logger LOG(L"ServiceAction");
const CString ServiceAction::JetServiceCommand(L"jetservice");

ServiceAction::ServiceAction()
  : SimpleConsoleAction(JetServiceCommand)
{
}


ServiceAction::~ServiceAction()
{
}

void ServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"    %s", myName);  
  writer->Write      (L"      internal, called to run as windows service");  
}

void ServiceAction::JetServiceMain(const Argz* az, const ServiceSettings* settings, DWORD dwArgc, LPTSTR *lpszArgv) {  
  SimpleRunServiceSettings rs(settings);
  Argz saz(dwArgc, lpszArgv);
  ServiceMain(&rs).JetServiceMain(&saz);
}

int ServiceAction::ExecuteAction(const Argz* argz, const ServiceSettings* settings) {
  CString serviceName = settings->getServiceName();
  LPWSTR buff = serviceName.GetBuffer();

  myState.setState(GetInstanceId(), this, argz, settings);
  SERVICE_TABLE_ENTRY tbl[] = {
    {buff, myState.getFunction(GetInstanceId()) },
    {NULL, NULL}
  };

  if (!StartServiceCtrlDispatcher(tbl)) {
    LOG.LogWarn(L"Failed to register service");
    return 1;
  }

  return 0;
}

int ServiceAction::GenerateServiceCommandLine(const Argz* argz, CString& result) {
  result = L"";

  const DWORD sz = 65535;
  TCHAR moduleNameBuff[sz+1];
  DWORD n = GetModuleFileName(NULL, moduleNameBuff, sz);

  if (n <= 0 || n >= sz) {
    LOG.LogErrorFormat(L"Failed to get executing module name");
    return 1;
  }

  LOG.LogDebugFormat(L"Executing process path: %s", moduleNameBuff);

  CString settings;
  int ret = GetSettingsFile(argz, settings);
  if (ret != 0) return ret;

  LOG.LogDebugFormat(L"Specified settings file: %s", settings);
  
  CString path;
  path.Append(L"\"");
  path.Append(moduleNameBuff);
  path.Append(L"\"");
  path.Append(L" ");
  path.Append(JetServiceCommand);
  path.Append(L" ");  
  path.Append(L"\"");
  path.Append(argz->MakeArgument(SettingsKeyName, settings));
  path.Append(L"\"");
  
  LOG.LogDebugFormat(L"Generated service command: %s", path);
  result = path;
  return 0;
}