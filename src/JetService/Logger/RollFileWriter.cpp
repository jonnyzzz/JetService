#include "StdAfx.h"
#include "RollFileWriter.h"
#include <share.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Logger.h"

const Logger LOG(L"RollFileWriter");

RollFileWriter::RollFileWriter()
  : myMaxSize(3 * 1024 * 1024)
  , myMaxNum(3)
  , myFileName(L"")
  , myFileStream(NULL)
  , myFileDescriptor(0)
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

  //NOTE: It's not possible to close log file as it's handle
  //NOTE: may have beed inherited by child process
  //NOTE: the easiest way is to reset file length to
  //NOTE: emulate reopen

  //Move older, closed files
  for (int older = myMaxNum, newer = older-1; newer >= 1; older--, newer--) {
    //first remove older file, if it exists
    DeleteFile(GetLogFileName(older));
    MoveFile(GetLogFileName(newer), GetLogFileName(older));
  }

  //remove file .1
  DeleteFile(GetLogFileName(1));
  //copy currect log to file.1. 
  //This is possible as file is opened with share_read
  CopyFile(GetLogFileName(), GetLogFileName(1), FALSE);
  
  //cleanup current file  
  if (myFileDescriptor > 0) {
    _chsize(myFileDescriptor, 0);
  }
}

void RollFileWriter::WriteLine(const CString& prefix, const CString& line) {
  if (myFileStream == NULL) return;

  CString time;
  FormatTimestamp(time);      
  fwprintf(myFileStream, L"%s%s%s\n", time, prefix, line);
  fflush(myFileStream);  

  RotateLogsIfNeeded();
}

void RollFileWriter::SetOutputFile(const CString& file) {  
  if (myFileStream != NULL) {  
    fclose(myFileStream);
    _close(myFileDescriptor);
    myFileDescriptor = 0;
    myFileStream = NULL;
    myFileName = L"";
  }

  int fd;
  if (0 != _wsopen_s(&fd, file, _O_WRONLY | _O_CREAT | _O_APPEND | _O_NOINHERIT | _O_U8TEXT, _SH_DENYWR, _S_IREAD | _S_IWRITE)) {
    LOG.LogWarnFormat(L"Failed to open settings file %s", file);
    return;
  }

  FILE* pfile = _wfdopen(fd, L"a");
  if (pfile == NULL) {
    LOG.LogWarnFormat(L"Failed to open settings file %s", file);
    _close(fd);
    return;
  }

  myFileDescriptor = fd;
  myFileStream = _wfdopen(fd, L"a");
  myFileName = file;
    
  LOG.LogInfoFormat(L"Logging will be redirected to: %s", myFileName);  
}
