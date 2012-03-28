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

void ServiceAction::JetServiceMain(const Argz* az, const RunServiceSettings* settings, DWORD dwArgc, LPTSTR *lpszArgv) {  
  LOG.LogDebug(L"Executing JetServiceMain");

  CString settingsFile;
  if (0 != GetSettingsFile(az, settingsFile)) {
    LOG.LogError(L"Failed to get path to settings file");
    return;
  }
  
  Argz saz(dwArgc, lpszArgv);
  ServiceMain(settings).JetServiceMain(&saz);
}

int ServiceAction::ExecuteAction(const Argz* argz, const RunServiceSettings* settings) {
  LOG.LogDebug(L"ExecuteAction entered");

  CString serviceName = settings->getServiceName();
  LPWSTR buff = serviceName.GetBuffer();

  bool set = myState.setState(GetInstanceId(), this, argz, settings);
  LPSERVICE_MAIN_FUNCTION main = myState.getFunction(GetInstanceId());
  if (!set || main == NULL) {
    LOG.LogErrorFormat(L"Failed to prepare Service Main function");
    return 1;
  }

  SERVICE_TABLE_ENTRY tbl[] = {
    {buff,  main},
    {NULL, NULL}
  };

  if (argz->IsServiceMockDebug()) {    
    tbl[0].lpServiceProc(0, NULL);
  } else {
    LOG.LogDebug(L"Calling StartServiceCtrlDispatcher");
    if (!StartServiceCtrlDispatcher(tbl)) {
      LOG.LogWarn(L"Failed to register service");
      return 1;
    }
  }

  LOG.LogDebug(L"Exit ExecuteAction");
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

  CString logFile;
  if (argz->GetLogFile(logFile)) {
    path.Append(L" ");
    path.Append(L"\"");
    path.Append(argz->MakeArgument(L"LogFile", logFile));
    path.Append(L"\"");
  }

  if (argz->IsDebug()) {
    path.Append(L" /debug ");
  }
  
  LOG.LogDebugFormat(L"Generated service command: %s", path);
  result = path;
  return 0;
}