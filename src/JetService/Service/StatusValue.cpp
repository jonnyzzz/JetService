#include "StdAfx.h"
#include "StatusValue.h"

StatusValue::StatusValue(const CString& name, DWORD state, bool isFinal, DWORD waitHint)
  : myName(name)
  , myState(state)
  , myIsFinal(isFinal)
  , myWaitHint(waitHint) {
}

StatusValue::~StatusValue() {
}

const CString& StatusValue::GetName() const {
  return myName;
}

DWORD StatusValue::GetState() const {
  return myState;
}

DWORD StatusValue::GetWaitHint() const {
  return max(1, myWaitHint);
}

bool StatusValue::IsRepeating() const {
  return myWaitHint > 0;
}

bool StatusValue::IsFinalEvent() const {
  return myIsFinal;
}


const StatusValue STARTING(L"starting", SERVICE_START_PENDING, false, 5000);
const StatusValue RUNNING(L"running", SERVICE_RUNNING);
const StatusValue STOPPING(L"stopping", SERVICE_STOP_PENDING, false, 5000);
const StatusValue STOPPED(L"stopped", SERVICE_STOPPED, true);

const StatusValue* StatusValue::STARTING=&::STARTING;
const StatusValue* StatusValue::RUNNING=&::RUNNING;
const StatusValue* StatusValue::STOPPING=&::STOPPING;
const StatusValue* StatusValue::STOPPED=&::STOPPED;

