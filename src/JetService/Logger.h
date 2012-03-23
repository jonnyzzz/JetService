#pragma once

enum LoggerSuverity {
	LogSDebug = 1,
	LogSInfo = 2,
	LogSWarn = 3,
	LogSError = 4
};

class Logger
{
private:
	static LoggerSuverity ourSuverity;

private:
	static bool ToLog(LoggerSuverity suv);

public:
	static void SetSuverity(LoggerSuverity suv);

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
  void LogDebug(const CString message, const CString m2) const;
  void LogDebug(const CString message, const CString m2, const CString m3) const;
  void LogDebugFormat(const CString message, ...) const;

	void LogInfo(const CString message) const;
  void LogInfo(const CString message, const CString m2) const;
  void LogInfo(const CString message, const CString m2, const CString m3) const;
  void LogInfoFormat(const CString message, ...) const;

  void LogWarn(const CString message) const;	
  void LogWarnFormat(const CString message, ...) const;	

	void LogError(const CString message) const;
  void LogErrorFormat(const CString message, ...) const;
	

  CString GetLastError() const;

  static CString GetErrorText(DWORD win32Error);
private:
	static void Log(LoggerSuverity suv, CString prefix, CString message);
};
