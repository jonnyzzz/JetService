#pragma once
class Argz
{
public:
  Argz(int argc, _TCHAR* argv[]);  
  Argz(const Argz& az, int skip = 0);
  virtual ~Argz(void);

public:
  CString GetExecutableName() const;

  CString GetArgument(int idx) const;
  int GetArgumentCount() const;
  bool HasArgument(const CString& text) const;  
  bool GetNamedArgument(const CString& text, CString& dest) const;
  bool GetBooleanArgument(const CString& name, bool def) const;

  Argz SkipFirstParameter() const;

public:
  bool IsDebug() const;
  bool IsServiceMockDebug() const;
  bool GetLogFile(CString& logFile) const;

  static CString MakeKey(const CString& name);
  static CString MakeArgument(const CString& name, const CString& value);
  static CString MakeBoolArgument(const CString& name, bool val = false);
private:
  const int myArgc;
  const CString myExecutableName;
  CString** const myArgv;
};

