#include "StdAfx.h"
#include "ServiceAction.h"
#include "Logger.h"

const Logger LOG(L"ServiceAction");

ServiceAction::ServiceAction()
  : SimpleConsoleAction(L"jetservice")
{
}


ServiceAction::~ServiceAction()
{
}



void ServiceAction::PrintUsage(ConsoleWriter* writer) {
  writer->WriteFormat(L"  %s", myName);  
}

int ServiceAction::ExecuteAction(const Argz* argz, const ServiceSettings* settings) {
  return 1;
}
