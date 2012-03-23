#include "StdAfx.h"
#include "ServiceCommand.h"

const SC_Right SC_Right::ALL_ACCESS = SC_Right(L"All access", SERVICE_ALL_ACCESS);
const SC_Right SC_Right::CHANGE_CONFIG = SC_Right(L"Change config", SERVICE_CHANGE_CONFIG);
const SC_Right SC_Right::INTERROGATE = SC_Right(L"Interrogate", SERVICE_INTERROGATE);
const SC_Right SC_Right::QUERY_CONFIG = SC_Right(L"Query config", SERVICE_QUERY_CONFIG);
const SC_Right SC_Right::START = SC_Right(L"Start", SERVICE_START);
const SC_Right SC_Right::STOP = SC_Right(L"Stop", SERVICE_STOP);
const SC_Right SC_Right::USER_DEFINED = SC_Right(L"User defined", SERVICE_USER_DEFINED_CONTROL);

ServiceCommand::ServiceCommand(const ServiceSettings* settings, const SCM_Right& right) 
  : mySettings(settings)
  , ServiceControlManagerCommand(right)
{
}


ServiceCommand::~ServiceCommand(void)
{
}
