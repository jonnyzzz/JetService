#include "StdAfx.h"
#include "Argz.h"

Argz::Argz(int argc, _TCHAR* argv[]) : myArgc(argc-1), myArgv(new CString*[argc]){  
  //Normally, 0-th argument is the executable name
  for(int i = 1; i < argc; i++) {    
    myArgv[i-1] = new CString(argv[i]);    
  }
  myArgv[myArgc] = NULL;
}
 
Argz::~Argz() {
  delete myArgv;
}

int Argz::GetArgumentCount() {
  return myArgc;
}

CString Argz::GetArgument(int c) {
  if (c < 0 || c >= myArgc) return CString(L"");
  return CString(*(myArgv[c]));
}

bool Argz::HasArgument(const CString& text) {
  for(int i = 0; i < GetArgumentCount(); i++) {
    if (text == GetArgument(i)) return true;
  }
  return false;
}


bool Argz::GetNamedArgument(const CString& text, CString& dest) {
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

