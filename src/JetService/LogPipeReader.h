#pragma once

#include "PipeReader.h"
#include "Logger.h"

class LogPipeReader : public ReaderCallback, private Logger
{
public:
  LogPipeReader(CString category);
  virtual ~LogPipeReader();

public:
  virtual void ProcessNextPortion(const CString& text);

};

