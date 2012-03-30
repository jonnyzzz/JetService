#pragma once

#include "PipeReader.h"
#include "Logger.h"

class LogPipeReader : public PipeReader, private Logger
{
public:
  LogPipeReader(CString category, HANDLE pipe, const InterruptHolder* interrupt = NULL);
  virtual ~LogPipeReader();

public:
  virtual void ProcessNextPortion(const CString& text);

};

