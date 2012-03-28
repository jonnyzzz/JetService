#pragma once

#include "Command.h"

namespace rapidxml {
  template<typename Ch> class xml_document;
  template<typename Ch> class xml_node;
}

class XmlFileSettings : public Command {
public:
  XmlFileSettings(const CString& configFilePath);
  virtual ~XmlFileSettings();

public:
  virtual int executeCommand();
  virtual int executeCommand(rapidxml::xml_document<TCHAR>* doc) = 0;

protected:
  CString nodeText(rapidxml::xml_node<TCHAR>* node);

private:
  const CString myConfigFile;

};
