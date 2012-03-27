#include "StdAfx.h"
#include "Argz.h"

CString getExecutableFileName(CString file) {  
  int p1 = file.ReverseFind(L'\\');
  int p2 = file.ReverseFind(L'\\');

  int p = max(p1,p2);
  if (p <= 0) return file;
  return file.Right(file.GetLength() - p - 1);
}

Argz::Argz(int argc, _TCHAR* argv[]) 
  : myArgc(max(0, argc-1))
  , myArgv(new CString*[myArgc + 1])
  , myExecutableName(getExecutableFileName(argc <= 0 ? L"JetService.exe" : argv[0]))  {  
  //Normally, 0-th argument is the executable name
  for(int i = 1; i < argc; i++) {    
    myArgv[i-1] = new CString(argv[i]);    
  }
  myArgv[myArgc] = NULL;
}

Argz::Argz(const Argz& az, int skip) 
  : myArgc(max(0, az.myArgc-skip))
  , myArgv(new CString*[myArgc+1])
  , myExecutableName(az.myExecutableName) {  
  //Normally, 0-th argument is the executable name
  for(int i = 0; i < myArgc; i++) {    
    myArgv[i] = new CString(az.GetArgument(i+skip));
  }
  myArgv[myArgc] = NULL;
}

Argz::~Argz() {
  delete myArgv;
}

int Argz::GetArgumentCount() const {
  return myArgc;
}

CString Argz::GetArgument(int c) const {
  if (c < 0 || c >= myArgc) return CString(L"");
  return CString(*(myArgv[c]));
}

CString Argz::GetExecutableName() const {
  return myExecutableName;
}

bool Argz::HasArgument(const CString& text) const {
  for(int i = 0; i < GetArgumentCount(); i++) {
    CString arg = GetArgument(i);
    if (text.CompareNoCase(arg) == 0) return true;
  }
  return false;
}

bool Argz::GetNamedArgument(const CString& text, CString& dest) const {
  CString pat = (MakeKey(text) + L"=").MakeLower();
  for(int i = 0; i < GetArgumentCount(); i++) {
    CString arg = GetArgument(i);
    if (arg.MakeLower().Find(pat) == 0) {
      dest = arg.Right(arg.GetLength() - pat.GetLength());
      return true;
    }
  }
  return false;
}

Argz Argz::SkipFirstParameter() const {
  return Argz(*this, 1); 
}

CString Argz::MakeKey(const CString& name) {
  return L"/" + name;
}

CString Argz::MakeArgument(const CString& name, const CString& value) {
  return MakeKey(name) + L"=" + value;
}


bool Argz::IsDebug() const {
  return HasArgument(L"/debug");
}

bool Argz::IsServiceMockDebug() const {
  return HasArgument(L"/debugservice");
}

bool Argz::GetLogFile(CString& logFile) const {
  logFile = L"";
  CString file;
  if (!GetNamedArgument(L"LogFile", file)) return false;
  
  const int sz=65535;
  TCHAR buff[sz+1];
  int n = GetFullPathName(file, sz, buff, NULL);
  if (n <= 0 || n >= sz) return false;
  logFile.Append(buff);
  return true;
}