#include "stdafx.h"
#include "Logger.h"
#include <share.h>
#include <stdarg.h>


class LoggerCriticalSection {
public:
  LoggerCriticalSection() {
    InitializeCriticalSection(&mySection);
  }
  ~LoggerCriticalSection() {
    DeleteCriticalSection(&mySection);
  }
private:
  //disabled copy constructor
  LoggerCriticalSection(const LoggerCriticalSection& c) {}

public:
  void EnterCriticalSection() {
    ::EnterCriticalSection(&mySection);
  }
  void LeaveCriticalSection() {
    ::LeaveCriticalSection(&mySection);
  }

private:
  CRITICAL_SECTION mySection;
};


LoggerSuverity Logger::ourSuverity = LogSError;
RollFileWriter Logger::ourFileWriter;
LoggerCriticalSection Logger::ourCriticalSection;

Logger::Logger(CString prefix) : myPrefix(prefix)
{	
}

Logger::~Logger()
{
}

bool Logger::IsDebugEnabled() {
	return Logger::ToLog(LogSDebug);
}

bool Logger::IsWarnEnabled() {
	return Logger::ToLog(LogSInfo);
}

bool Logger::IsInfoEnabled() {
	return Logger::ToLog(LogSInfo);
}

  
void Logger::LogDebug(const CString message) const {
	Logger::Log(LogSDebug, myPrefix, message);
}

void Logger::LogDebugFormat(const CString message, ...) const {
  if (!IsDebugEnabled()) return;

  CString str;
  va_list args;
  va_start(args,message);
  str.FormatV(message, args);  
  va_end(args);  
  LogDebug(str);
}

void Logger::LogInfo(const CString message) const {
	Logger::Log(LogSInfo, myPrefix, message);
}

void Logger::LogInfoFormat(const CString message, ...) const {
  if (!IsInfoEnabled()) return;

  CString str;
  va_list args;
  va_start(args,message);
  str.FormatV(message, args);  
  va_end(args);  
  LogInfo(str);
}

void Logger::LogWarn(const CString message) const {
	Logger::Log(LogSWarn, myPrefix, message);
}

void Logger::LogWarnFormat(const CString message, ...) const {
  if (!IsWarnEnabled()) return;

  CString str;
  va_list args;
  va_start(args,message);
  str.FormatV(message, args);  
  va_end(args);  
  LogWarn(str);
}

void Logger::LogError(const CString message) const {
	Logger::Log(LogSError, myPrefix, message);
}

void Logger::LogErrorFormat(const CString message, ...) const {
  CString str;
  va_list args;
  va_start(args,message);
  str.FormatV(message, args);  
  va_end(args);  
  LogError(str);
}

bool Logger::ToLog(LoggerSuverity suv) {
	return (int)ourSuverity <= (int)suv;
}

void Logger::SetSuverity(LoggerSuverity suv) {
	Logger::ourSuverity = suv;
}

CString Logger::GetLastError() const {
  return GetErrorText(::GetLastError());
}

CString Logger::GetErrorText(DWORD win32Error) {
  const DWORD sz = 65535;
  TCHAR buff[sz+1];

  DWORD n = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM,
    NULL, 
    win32Error, 
    LANG_NEUTRAL,
    buff,
    sz, 
    NULL);

  if (n > 0) {
    CString str;
    str.Append(buff);
    str.Trim();
    str.AppendFormat(L" (%d)", win32Error);
    return str;
  } else {
    CString txt;
    txt.Format(L"%d", win32Error);
    return txt;
  }
}

void Logger::Log(LoggerSuverity suv, const CString& prefix, const CString& message) {    
  if (!Logger::ToLog(suv)) return;
  CString log;
  const int NAME_SZ = 33;

	log.Append(prefix);
  if (log.GetLength() > NAME_SZ-1) {
    log = log.Left(NAME_SZ-1);
  }

  while (log.GetLength() < NAME_SZ) log.AppendChar(L' ');
	switch(suv) {
		case LogSDebug:
      log.Append(L"[Debug] ");
			break;
		case LogSInfo:
			log.Append(L" [Info] ");
			break;
		case LogSWarn:
			log.Append(L" [Warn] ");
			break;
		case LogSError:
			log.Append(L"[Error] ");
			break;
	}		
	log.Append(message);
	log.Append(L"\n");

  ourCriticalSection.EnterCriticalSection();
  LogMessageInternal(log);
  ourCriticalSection.LeaveCriticalSection();	
}

void Logger::LogMessageInternal(const CString& log) {
  wprintf_s(L"%s", log);
  ourFileWriter.WriteLine(log);
}

void Logger::SetLogFile(const CString& file) {
  ourFileWriter.SetOutputFile(file);
}
