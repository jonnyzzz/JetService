#pragma once

#include "XmlFileSettings.h"
#include "ServiceSettings.h"

class SimpleServiceTaskSettings;

class FileTaskSettings : public XmlFileSettings
{
public:
  FileTaskSettings(const ServiceSettings* runSettings);
  virtual ~FileTaskSettings();

public:
  virtual int executeCommand(rapidxml::xml_document<TCHAR>* doc);

public:  
  virtual int executeCommand(const ServiceTaskSettings* settings) = 0;

private:
  int GetBaseDirectory(CString& baseFile);

  int parseProgramPath(CString baseDir, rapidxml::xml_node<TCHAR>* execution, SimpleServiceTaskSettings* settings);
  int parseProgramArgs(CString baseDir, rapidxml::xml_node<TCHAR>* execution, SimpleServiceTaskSettings* settings);
  int parseProgramDir(CString baseDir, rapidxml::xml_node<TCHAR>* execution, SimpleServiceTaskSettings* settings);
  int parseProgramStopTimeout(rapidxml::xml_node<TCHAR>* execution, SimpleServiceTaskSettings* settings);

private:
  const ServiceSettings* const myRunSettings;

};

