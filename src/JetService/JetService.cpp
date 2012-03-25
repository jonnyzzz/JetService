// JetService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"
#include "FileServiceSettings.h"
#include "Logger.h"

const Logger LOG(L"main");

int _tmain(int argc, _TCHAR* argv[])
{
  Argz az(argc, argv);

  if (az.HasArgument(L"/debug")) {
    Logger::SetSuverity(LoggerSuverity::LogSDebug);
  } else {
    Logger::SetSuverity(LoggerSuverity::LogSInfo);
  }

  LOG.LogInfo(L"Starting JetService...");

  CString file;
  if (!az.GetNamedArgument(L"settings", file)) {
    LOG.LogErrorFormat(L"Failed to find configuration file");
    return 1;
  }

  FileServiceSettings set(file);
  return set.executeCommand();
  
	return 0;
}

