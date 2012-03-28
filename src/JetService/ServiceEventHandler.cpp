#include "StdAfx.h"
#include "ServiceEventHandler.h"


ServiceEventHandler::ServiceEventHandler(const CString& eventName, DWORD controlEventId)
  : myName(eventName)
  , myControlEventId(controlEventId)
{
}


ServiceEventHandler::~ServiceEventHandler(void)
{
}

const CString& ServiceEventHandler::GetEventName() const {
  return myName;
}

DWORD ServiceEventHandler::GetControlEventId() const {
  return myControlEventId;
}

