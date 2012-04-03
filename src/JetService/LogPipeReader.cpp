#include "StdAfx.h"
#include "LogPipeReader.h"


LogPipeReader::LogPipeReader(CString category)
  : Logger(category)
{
}


LogPipeReader::~LogPipeReader()
{
}

void LogPipeReader::ProcessNextPortion(const CString& text)  {
  LogInfo(text);
}

