#include "StdAfx.h"
#include "InterruptHolder.h"



InterruptHolder::InterruptHolder(const InterruptHolder* parent)
  : myStopFlag(false)
  , myParent(parent)
{
}


InterruptHolder::~InterruptHolder()
{
}


bool InterruptHolder::IsInterrupted() const {   
  return myStopFlag || ((myParent != NULL) && myParent->IsInterrupted()); 
}

void InterruptHolder::Interrupt() { 
  myStopFlag = true;   
}

CString InterruptHolder::IsInterruptedString() const {
  return IsInterrupted() ? L"true" : L"false";
}
