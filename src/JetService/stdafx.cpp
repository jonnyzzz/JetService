// stdafx.cpp : source file that includes just the standard includes
// JetService.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <stdarg.h>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

CString CreateFormatted(const CString tmpl, ...) {
  CString str;
  va_list args;
  va_start(args,tmpl);
  str.FormatV(tmpl, args);  
  va_end(args);  

  return str;
}


bool CopyStringChars(const CString& text, TCHAR* buff, int sz) {
  if (sz <= 1) return false;
  *buff = L'\0';
  if (text.GetLength() >= sz-1) return false;
  LPCTSTR tmp = text;
  CString::CopyChars(buff, sz-1, text, text.GetLength());
  buff[text.GetLength()] = L'\0';
  return true;
}

