#include "StdAfx.h"
#include "Argz.h"

Argz::Argz(int argc, _TCHAR* argv[]) 
  : myArgc(argc-1)
  , myArgv(new CString*[argc])
  , myExecutableName(argc <= 0 ? L"JetService.exe" : argv[0])  {  
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
    myArgv[i] = new CString(az.GetArgument(i));
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
    if (text == GetArgument(i)) return true;
  }
  return false;
}


bool Argz::GetNamedArgument(const CString& text, CString& dest) const {
  CString pat = L"/" + text + L"=";
  for(int i = 0; i < GetArgumentCount(); i++) {
    CString arg = GetArgument(i);
    if (arg.Find(pat) == 0) {
      dest = arg.Right(arg.GetLength() - pat.GetLength());
      return true;
    }
  }
  return false;
}

Argz Argz::SkipFirstParameter() const {
  return Argz(*this, 1); 
}

