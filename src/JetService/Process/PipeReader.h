#pragma once

#include "InterruptHolder.h"

class ReaderCallback {
public:
  ReaderCallback();
  virtual ~ReaderCallback();

public:
  ///implementation must copy data
  virtual void ProcessNextPortion(const CString& text) = 0;
};

class PipeReader : public InterruptHolder
{
public:
  PipeReader(ReaderCallback* readerCallback, HANDLE pipe, InterruptHolder* interrupt = NULL);
  virtual ~PipeReader(void);

public:
  void WaitForExit();

public:
  DWORD ThreadProc();

private:
  void ProcessBuffer(CString& buffer);    
  void ProcessNextPortion(const CString& text);

private:  
  ReaderCallback* const myReaderCallback;
  const HANDLE myPipe;

  HANDLE myThread;
  DWORD myThreadId;
};

