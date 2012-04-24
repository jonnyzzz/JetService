#pragma once

#include "RollFileWriter.h"

enum LoggerSuverity {
	LogSDebug = 1,
	LogSInfo = 2,
	LogSWarn = 3,
	LogSError = 4
};

class LoggerCriticalSection;

class Logger
{

public:
	static void SetSuverity(LoggerSuverity suv);
  static void SetLogFile(const CString& file);

private:
	const CString myPrefix;

public:
	Logger(const CString prefix);	
	virtual ~Logger(void);

public:
	bool static IsDebugEnabled();
	bool static IsWarnEnabled();
	bool static IsInfoEnabled();
	
	void LogDebug(const CString message) const;  
  void LogDebugFormat(const CString message, ...) const;

	void LogInfo(const CString message) const;
  void LogInfoFormat(const CString message, ...) const;

  void LogWarn(const CString message) const;	
  void LogWarnFormat(const CString message, ...) const;	

	void LogError(const CString message) const;
  void LogErrorFormat(const CString message, ...) const;
	
public:
  CString GetLastError() const;
  static CString GetErrorText(DWORD win32Error);

private:
	static void Log(LoggerSuverity suv, const CString& prefix, const CString& message);
	static bool ToLog(LoggerSuverity suv);  
  static void LogMessageInternal(const CString& prefix, const CString& message);

private:
	static LoggerSuverity ourSuverity;
  static LoggerCriticalSection ourCriticalSection;
  static RollFileWriter ourFileWriter;
};
