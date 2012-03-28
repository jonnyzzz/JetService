#include "StdAfx.h"
#include "ServiceEventContext.h"


ServiceEventContext::ServiceEventContext()
{
}

ServiceEventContext::~ServiceEventContext()
{
}


bool ServiceEventContext::IsValid() const {
  return GetServiceStatus() != NULL;
}

////////////

ServiceEventContextEx::ServiceEventContextEx() 
  : myServiceStatus(NULL) {
}

ServiceEventContextEx::~ServiceEventContextEx() {
}

ServiceStatus* ServiceEventContextEx::GetServiceStatus() const {
  return myServiceStatus;
}

void ServiceEventContextEx::SetServiceStatus(ServiceStatus* status) {
  myServiceStatus = status;
}

