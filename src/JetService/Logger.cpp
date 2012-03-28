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
FILE* Logger::ourFileStream = NULL;
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
void Logger::LogDebug(const CString message, const CString m2) const {
  if (!IsDebugEnabled()) return;
  LogDebug(message + m2);
}
void Logger::LogDebug(const CString message, const CString m2, const CString m3) const {
  if (!IsDebugEnabled()) return;
  LogDebug(message + m2 + m3);
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
void Logger::LogInfo(const CString message, const CString m2) const {
  if (!IsInfoEnabled()) return;
  LogInfo(message + m2);
}
void Logger::LogInfo(const CString message, const CString m2, const CString m3) const {
  if (!IsInfoEnabled()) return;
  LogInfo(message + m2 + m3);
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
  const int NAME_SZ = 33;

	if (Logger::ToLog(suv)) {
    CString log;    
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
				log.Append(L"  [Err] ");
				break;
		}		
		log.Append(message);
		log.Append(L"\n");

    ourCriticalSection.EnterCriticalSection();
    {
      wprintf_s(L"%s", log);
      if (ourFileStream != NULL) {
        fwprintf(ourFileStream, L"%s", log);
        fflush(ourFileStream);
      }
    }
    ourCriticalSection.LeaveCriticalSection();
	}
}

void Logger::SetLogFile(const CString& file) {
  Logger LOG(L"Logger#SetLogFile");
  if (ourFileStream != NULL) {
    fclose(ourFileStream);
    ourFileStream = NULL;
  }

  FILE* stream =_wfsopen(file, L"a", _SH_DENYWR);
  if (stream == NULL) {
    LOG.LogWarnFormat(L"Failed to open settings file %s", file);
    return;
  }
  
  LOG.LogInfoFormat(L"Logging will be redirected to: %s", file);
  ourFileStream = stream;
  fwprintf(ourFileStream, L"\n\n\n");
  fflush(ourFileStream);
}
