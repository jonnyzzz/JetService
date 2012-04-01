#include "StdAfx.h"
#include "RollFileWriter.h"
#include <share.h>
#include "Logger.h"

const Logger LOG(L"RollFileWriter");

RollFileWriter::RollFileWriter()
{
}


RollFileWriter::~RollFileWriter()
{
}

void RollFileWriter::FormatTimestamp(CString& buff) {
  SYSTEMTIME time;
  GetLocalTime(&time);
  buff.Format(L"[%04d-%02d-%02d %02d:%02d:%02d,%03d] ", 
    (DWORD)time.wYear, (DWORD)time.wMonth, (DWORD)time.wDay, 
    (DWORD)time.wHour, (DWORD)time.wMinute, (DWORD)time.wSecond, (DWORD)time.wMilliseconds);
}


void RollFileWriter::WriteLine(const CString& line) {
  if (myFileStream == NULL) return;

  CString time;
  FormatTimestamp(time);      
  
  fwprintf(myFileStream, L"%s%s", time, line);
  fflush(myFileStream);     
}


void RollFileWriter::SetOutputFile(const CString& file) {  
  if (myFileStream != NULL) {
    fclose(myFileStream);
    myFileStream = NULL;
  }

  FILE* stream =_wfsopen(file, L"a", _SH_DENYWR);
  if (stream == NULL) {
    LOG.LogWarnFormat(L"Failed to open settings file %s", file);
    return;
  }
    
  LOG.LogInfoFormat(L"Logging will be redirected to: %s", file);
  myFileName = file;
  myFileStream = stream;
  
  WriteLine(L"\n\n\n");  
}