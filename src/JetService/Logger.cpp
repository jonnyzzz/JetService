#include "stdafx.h"
#include "Logger.h"
#include <stdarg.h>

LoggerSuverity Logger::ourSuverity = LogSError;

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
  CString txt;
  txt.Format(L"%d", win32Error);
  return txt;
}

void Logger::Log(LoggerSuverity suv, CString prefix, CString message) {    
	if (Logger::ToLog(suv)) {
    CString log;    
		log.Append(prefix);
    log.Append(L" [");
		switch(suv) {
			case LogSDebug:
        log.Append(L"Debug");
				break;
			case LogSInfo:
				log.Append(L"Info");
				break;
			case LogSWarn:
				log.Append(L"Warn");
				break;
			case LogSError:
				log.Append(L"Error");
				break;
		}
		log.Append(L"]");
    while (log.GetLength() < 20) log.AppendChar(L' ');
		log.Append(message);
		log.Append(L"\r\n");
    wprintf_s(log);
	}
}