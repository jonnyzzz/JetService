#pragma once

#include "consolewriter.h"

class SimpleConsoleWriter : public ConsoleWriter
{
public:
  SimpleConsoleWriter();
  virtual ~SimpleConsoleWriter();


public:
  virtual void Write(const CString& str);
};

