#pragma once

#include "InterruptHolder.h"

class PipeReader : public InterruptHolder
{
public:
  PipeReader(HANDLE pipe, const InterruptHolder* interrupt = NULL);
  virtual ~PipeReader(void);

public:
  DWORD ThreadProc();

protected:
  ///implementation must copy data
  virtual void ProcessNextPortion(const CString& text) = 0;

private:
  void ProcessBuffer(CString& buffer);    

private:  
  const HANDLE myPipe;

  HANDLE myThread;
  DWORD myThreadId;
};

