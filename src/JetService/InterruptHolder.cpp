#include "StdAfx.h"
#include "InterruptHolder.h"



InterruptHolder::InterruptHolder()
  : myStopFlag(false)
{
}


InterruptHolder::~InterruptHolder()
{
}


bool InterruptHolder::IsInterrupted() const { 
  return myStopFlag; 
}

void InterruptHolder::Interrupt() { 
  myStopFlag = true; 
}

