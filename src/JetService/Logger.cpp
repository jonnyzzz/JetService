#include "stdafx.h"
#include "Logger.h"

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

void Logger::LogInfo(const CString message) const {
	Logger::Log(LogSInfo, myPrefix, message);
}

void Logger::LogWarn(const CString message) const {
	Logger::Log(LogSWarn, myPrefix, message);
}

void Logger::LogError(const CString message) const {
	Logger::Log(LogSError, myPrefix, message);
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
    log.Append(L" ");
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
		log.Append(L" - ");
		log.Append(message);
		log.Append(L"\r\n");
    wprintf_s(log);
	}
}