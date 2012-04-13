#pragma once

class ConsoleWriter
{
public:
  ConsoleWriter();
  virtual ~ConsoleWriter();

public:
  virtual void Write();
  virtual void Write(const CString& str) = 0;
  virtual void WriteFormat(const CString tmpl, ...);  
};

