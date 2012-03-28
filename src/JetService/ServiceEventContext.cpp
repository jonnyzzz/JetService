#include "StdAfx.h"
#include "ServiceEventContext.h"
#include "ServiceTask.h"

ServiceEventContext::ServiceEventContext()
{
}

ServiceEventContext::~ServiceEventContext()
{
}


bool ServiceEventContext::IsValid() const {
  return GetServiceStatus() != NULL
    && GetServiceTask() != NULL;
}

////////////

ServiceEventContextEx::ServiceEventContextEx() 
  : myServiceStatus(NULL)
  , myTask(NULL) {
}

ServiceEventContextEx::~ServiceEventContextEx() {
}

ServiceStatus* ServiceEventContextEx::GetServiceStatus() const {
  return myServiceStatus;
}

void ServiceEventContextEx::SetServiceStatus(ServiceStatus* status) {
  myServiceStatus = status;
}

ServiceTask* ServiceEventContextEx::GetServiceTask() const {
  return myTask;
}

void ServiceEventContextEx::SetServiceTask(ServiceTask* task) {
  myTask = task;
}

