#include "StdAfx.h"
#include "ConsoleWriter.h"


ConsoleWriter::ConsoleWriter(void)
{
}


ConsoleWriter::~ConsoleWriter(void)
{
}


void ConsoleWriter::Write() {
  Write(L"");
}

void ConsoleWriter::WriteFormat(const CString tmpl, ...) {
  CString str;
  
  va_list args;

  va_start(args, tmpl);
  str.FormatV(tmpl, args);  
  va_end(args);  
  
  Write(str);
}
