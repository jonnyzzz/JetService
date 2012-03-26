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

  Argz SkipFirstParameter() const;

  static CString MakeKey(const CString& name);

private:
  const int myArgc;
  const CString myExecutableName;
  CString** const myArgv;
};

