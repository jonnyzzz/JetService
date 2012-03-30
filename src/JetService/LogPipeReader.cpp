#include "StdAfx.h"
#include "LogPipeReader.h"


LogPipeReader::LogPipeReader(CString category, HANDLE pipe, const InterruptHolder* interrupt)
  : Logger(category)
  , PipeReader(pipe, interrupt)
{
}


LogPipeReader::~LogPipeReader()
{
}

void LogPipeReader::ProcessNextPortion(const CString& text)  {
  LogInfo(text);
}

