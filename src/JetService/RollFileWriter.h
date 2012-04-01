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
  void RotateLogsIfNeeded();
  CString GetLogFileName(int index = 0);

  void CloseFile();
  void ReopenFile();

private:
  CString myFileName;
  FILE* myFileStream;

  const int myMaxSize;
  const int myMaxNum;
};

