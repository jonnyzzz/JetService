#include "StdAfx.h"
#include "Argz.h"

Argz::Argz(int argc, _TCHAR* argv[]) : myArgc(argc), myArgv(new CString*[argc]){
  CString** strings = new CString*[argc];
  for(int i = 0; i < argc; i++) {    
    strings[i] = new CString(argv[i]);    
  }
}
 
Argz::~Argz() {
  delete myArgv;
}

int Argz::getArgumentCount() {
  return myArgc;
}

CString Argz::getArgument(int c) {
  if (c < 0 || c >= myArgc) return CString(L"");
  return *(myArgv[c]);
}


