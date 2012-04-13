#pragma once

#include "XmlFileSettings.h"
#include "ServiceSettings.h"


class FileServiceSettings : public XmlFileSettings
{
public:
  FileServiceSettings(const CString& configFilePath);
  virtual ~FileServiceSettings();

public:
  virtual int executeCommand(rapidxml::xml_document<TCHAR>* doc);

public:  
  virtual int executeCommand(const ServiceSettings* settings) = 0;

};

