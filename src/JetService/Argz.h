#pragma once
class Argz
{
public:
  Argz(int argc, _TCHAR* argv[]);
  virtual ~Argz(void);

public:
  CString GetArgument(int idx);
  int GetArgumentCount();

  bool HasArgument(const CString& text);
  
  bool GetNamedArgument(const CString& text, CString& dest);

private:
  const int myArgc;
  CString** const myArgv;
};

