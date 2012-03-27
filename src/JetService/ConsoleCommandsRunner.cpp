#include "StdAfx.h"
#include "ConsoleCommandsRunner.h"
#include "Logger.h"

const Logger LOG(L"ConsoleCommandsRunner");


ConsoleCommandsRunner::ConsoleCommandsRunner(ConsoleWriter* writer, Argz* argz, ConsoleAction** commands)
  : myWriter(writer)
  , myArgz(argz)
  , myActions(commands)
{
}


ConsoleCommandsRunner::~ConsoleCommandsRunner(void)
{
}


int ConsoleCommandsRunner::executeCommand() {
  CString logFile;
  if (myArgz->GetLogFile(logFile)) {
    Logger::SetLogFile(logFile);
  }
  if (myArgz->IsDebug()) {
    Logger::SetSuverity(LogSDebug);
    LOG.LogDebug(L"Debug logging is enabled");
  } else {
    Logger::SetSuverity(LogSInfo);
  }

  if (myArgz->IsServiceMockDebug()) {
    LOG.LogWarn(L"Service functions are mocked");
  }

  if (myArgz->GetArgumentCount() < 1) {
    LOG.LogWarn(L"Incorrect usage.");
    return usage();
  }

  CString command = myArgz->GetArgument(0);
  LOG.LogDebugFormat(L"Selected commnad: %s", command);
  Argz subArgz = myArgz->SkipFirstParameter();

  for(ConsoleAction** p = myActions; *p != NULL; p++) {
    ConsoleAction* action = *p;
    if (action->GetCommandName() == command) {
      return action->ExecuteAction(&subArgz);
    }
  }

  LOG.LogWarnFormat(L"Unknown command: %s", command);
  return usage();
}


int ConsoleCommandsRunner::usage() {
  myWriter->Write();
  myWriter->Write(L"Usage:");
  myWriter->WriteFormat(L"  %s <command> [/debug]", myArgz->GetExecutableName());
  myWriter->Write();
  myWriter->Write(L"  where <command> could be one of the following: ");
  for(ConsoleAction** p = myActions; *p != NULL; p++) {    
    (*p)->PrintUsage(myWriter);
    myWriter->Write();
  }

  return 1;

}
