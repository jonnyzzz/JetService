#pragma once

#include "XmlFileSettings.h"
#include "ServiceSettings.h"

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

private:
  const ServiceSettings* const myRunSettings;

};

