#include "StdAfx.h"
#include "ServiceAction.h"
#include "Logger.h"

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

int ServiceAction::ExecuteAction(const Argz* argz, const ServiceSettings* settings) {
  return 1;
}
