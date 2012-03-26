#include "StdAfx.h"
#include "SimpleConsoleWriter.h"
#include <stdarg.h>

SimpleConsoleWriter::SimpleConsoleWriter()
{
}


SimpleConsoleWriter::~SimpleConsoleWriter()
{
}

void SimpleConsoleWriter::Write(const CString& str) {
  wprintf(L"%s\r\n", str);
}
