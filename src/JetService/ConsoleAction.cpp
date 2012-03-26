#include "StdAfx.h"
#include "ConsoleAction.h"


ConsoleAction::ConsoleAction(const CString& name)
  : myName(name)
{
}


ConsoleAction::~ConsoleAction(void)
{
}

CString ConsoleAction::GetCommandName() const {
  return myName;
}