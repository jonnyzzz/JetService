#include "StdAfx.h"
#include "RollFileWriter.h"
#include <share.h>
#include "Logger.h"

const Logger LOG(L"RollFileWriter");

RollFileWriter::RollFileWriter()
  : myMaxSize(1024)
  , myMaxNum(3)
  , myFileName(L"")
  , myFileStream(NULL)
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

CString RollFileWriter::GetLogFileName(int index) {
  if (index == 0) return myFileName;
  return CreateFormatted(L"%s.%d", myFileName, index);
}

void RollFileWriter::RotateLogsIfNeeded() {
  if (myFileStream == NULL) return;

  long sz = ftell(myFileStream);
  if (sz < 0) {
    //error occured
    //let's try to reopen logs
    SetOutputFile(myFileName);
    return;
  }

  if (sz < myMaxSize) {
    //not enuogh length
    return;
  }

  CloseFile();
  
  for (int older = myMaxNum, newer = older-1; newer >= 0; older--, newer--) {
    //first remove older file, if it exists
    DeleteFile(GetLogFileName(older));
    MoveFile(GetLogFileName(newer), GetLogFileName(older));
  }
  
  //remove current log file to ensure to start log from scratch
  DeleteFile(GetLogFileName());

  ReopenFile();
}

void RollFileWriter::WriteLine(const CString& line) {
  if (myFileStream == NULL) return;

  CString time;
  FormatTimestamp(time);      
  
  fwprintf(myFileStream, L"%s%s", time, line);
  fflush(myFileStream);  

  RotateLogsIfNeeded();
}

void RollFileWriter::SetOutputFile(const CString& file) {  
  CloseFile();

  myFileName = file;
  ReopenFile();
    
  LOG.LogInfoFormat(L"Logging will be redirected to: %s", myFileName);  
  WriteLine(L"\n\n\n");  
}


void RollFileWriter::CloseFile() {
  if (myFileStream == NULL) return;
  
  fclose(myFileStream);
  myFileStream = NULL;
}


void RollFileWriter::ReopenFile() {
  CString file = GetLogFileName();
  if (file.GetLength() == 0) return;
  
  FILE* stream =_wfsopen(file, L"a", _SH_DENYWR);
  if (stream == NULL) {
    LOG.LogWarnFormat(L"Failed to open settings file %s", file);
    return;
  }
  
  myFileStream = stream;
}
