#pragma once
class Argz
{
public:
  Argz(int argc, _TCHAR* argv[]);
  virtual ~Argz(void);

public:
  CString getArgument(int idx);
  int getArgumentCount();

private:
  const int myArgc;
  CString** const myArgv;
};

