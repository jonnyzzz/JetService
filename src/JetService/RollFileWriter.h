#pragma once

class RollFileWriter
{
public:
  RollFileWriter(void);
  virtual ~RollFileWriter(void);
private:
  RollFileWriter(const RollFileWriter&);

public:
  void WriteLine(const CString& line);

public:
  void SetOutputFile(const CString& file);

private:
  void FormatTimestamp(CString& buff);

private:
  CString myFileName;
  FILE* myFileStream;
};

